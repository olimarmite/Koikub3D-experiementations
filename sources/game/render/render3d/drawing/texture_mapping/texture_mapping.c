#include "draw_utils.h"
#include "structures.h"
#include "ressources_managers.h"
#include "render_3D.h"
#include <assert.h>


// t_vector4d project_point(t_3d_render *render, t_vector4d point)
// {
// 	t_vector4d transformed_point;

// 	int xscale = 512 / (render->scale_factor_x / 1.2);
// 	int yscale = 32 / (render->scale_factor_y / 1.2);



// 	transformed_point.x = -point.x * xscale / point.y;
// 	transformed_point.y = point.z * yscale / point.y;
// 	transformed_point.z = point.y;
// 	transformed_point.w = 0;
// 	// transformed_point.vec *= 16;
// 	transformed_point.vec += render->middle.vec;
// 	return (transformed_point);
// }

static inline void	draw_vertical_line_tiled(
			t_3d_render	*render,
			t_img_data	*image,
			int 		img_x,
			int 		screen_x,
			int 		screen_top,
			int			screen_bottom,
			double		tiled_factor,
			double		depth,
			t_vector4d	*normal,
			t_vector4d	*world_pos,
			double 		*wall_ceil,
			double 		*wall_floor

			)
{
	uint32_t	*img = (void *)image->addr;
	int			offset;
	int			offset_img;
	double		y = 0;

	double full_project_height = screen_bottom - screen_top;
	img_x = img_x % image->size.x;
	double factor  =  ((double)image->size.y * tiled_factor) / (double) (screen_bottom - screen_top);
	y = (int)fmax(0, render->top_array[screen_x] - screen_top); //fmax(screen_top, render->top_array[screen_x]);
	screen_bottom = fmin(screen_bottom, render->bottom_array[screen_x]);

	// double z_pos_factor = ((render->middle.y - screen_top) / (0 - *wall_ceil)) * *wall_ceil;
	while (screen_top + y < screen_bottom)
	{
		if (y + screen_top >= render->height)
			return;
		if (img_x > render->width)
			return;
		// if (y + screen_top < 0)
		// 	return;
		offset = (screen_top + y) * render->width + screen_x;
		offset_img = (((int)(factor * y) % image->size.y) * image->size.x) + img_x;

		// int yscale = 512 / (render->scale_factor_y / 1.2);
		double z_pos_factor = (y) / (double) (full_project_height);

		world_pos->z = (*wall_ceil * (1 - z_pos_factor) + *wall_floor * (z_pos_factor));
		// world_pos->z = ((z_pos_factor + y));// / image->size.y;
		render->buffers.color[offset].d = img[offset_img];
		render->buffers.depth[offset] = depth;
		render->buffers.world_pos[offset] = *world_pos;
		render->buffers.normal[offset] = *normal;
		++y;
	}
}



int	relative_segment_clip_front(t_segment_d *segment);

t_vector4d	project_point(t_3d_render *render, t_vector4d point);

typedef enum e_side
{
	SIDE_ON			= 0b0000,
	SIDE_LEFT		= 0b0001,
	SIDE_RIGHT		= 0b0010,
	SIDE_INTERSECT	= 0b0011,
}	t_side;

static t_side
	point_segment_side(t_segment_d *separator, t_vector4d *segment_point)
{
	t_vector4d	ab;
	t_vector4d	ap;
	double		cross_product;

	ab.vec = separator->point_b.vec - separator->point_a.vec;
	ap.vec = segment_point->vec - separator->point_a.vec;
	cross_product = ab.x * ap.y - ab.y * ap.x;
	if (cross_product > 0)
		return (SIDE_RIGHT);
	else if (cross_product < 0)
		return (SIDE_LEFT);
	else
		return (SIDE_ON);
}

void invert_if_needed(t_3d_render *render, t_segment_d *segment)
{
	t_vector4d	tmp;

	if (point_segment_side(segment, &render->camera->pos) == SIDE_LEFT)
	{
		tmp = segment->point_a;
		segment->point_a = segment->point_b;
		segment->point_b = tmp;
	}
}

double	calc_segment_size(t_segment_d *segment)
{
	return (sqrtf(powf(segment->point_b.x - segment->point_a.x, 2) + powf(segment->point_b.y - segment->point_a.y, 2)));
}

double	calc_wall_texture_repeat_factor_x(t_segment_d *segment)
{
	double segment_size = calc_segment_size(segment);

	return (segment_size);

}

double	calc_wall_texture_repeat_factor_y(t_segment_d *segment)
{
	return (fabs(segment->data.floor - segment->data.ceil)); /// 1);
}

double	calc_wall_texture_offset(__attribute_maybe_unused__ t_segment_d *segment)
{
	return 0.5;
}

typedef struct s_texture_mapping_data
{
	t_segment_d		*wall;
	t_segment_d		relative_segment;
	t_segment_d		clipped_relative_segment;
	double			texture_width;
	double			texture_tiling_factor_x;
	double			texture_tiling_factor_y;
	double			u0;
	double			u1;
	t_segment_d		projected_top;
	t_segment_d		projected_bot;
	double			coef_top;
	double			coef_bot;
	double			x;
	double			top;
	double			bot;
}	t_texture_mapping_data;


static inline void	calc_u(t_texture_mapping_data *data)
{
	double		relative_segment_width;
	const int	use_y = (fabs(data->clipped_relative_segment.point_b.x
				- data->clipped_relative_segment.point_a.x)
			<= fabs(data->relative_segment.point_a.y
				- data->relative_segment.point_b.y));

	relative_segment_width = data->relative_segment.point_b.vec[use_y]
		- data->relative_segment.point_a.vec[use_y];
	data->u0 = ((data->clipped_relative_segment.point_a.vec[use_y]
				- data->relative_segment.point_a.vec[use_y])
			* data->texture_width
			* data->texture_tiling_factor_x)
		/ relative_segment_width;
	data->u1 = ((data->clipped_relative_segment.point_b.vec[use_y]
				- data->relative_segment.point_a.vec[use_y])
			* data->texture_width
			* data->texture_tiling_factor_x)
		/ relative_segment_width;


	data->u0 += calc_wall_texture_offset(data->wall) * data->texture_width;
	data->u1 += calc_wall_texture_offset(data->wall) * data->texture_width;
}

// t_vector4d transform_camera_relative_point(t_vector4d point, t_camera *camera)
// {
// 	t_vector4d relative_point;

// 	point.vec = point.vec - camera->pos.vec;

// 	relative_point.x = point.x * camera->dir.y - point.y * camera->dir.x;
// 	relative_point.y = point.x * camera->dir.x + point.y * camera->dir.y;


// 	relative_point.z = point.z;
// 	relative_point.w = point.w;
// 	return (relative_point);
// }

t_vector4d reverse_transform_camera_relative_point(t_vector4d relative_point, t_camera *camera) {
    t_vector4d point;
    double det = camera->dir.x * camera->dir.x + camera->dir.y * camera->dir.y;

    if (det == 0) {
        // Handle the case where the determinant is zero (i.e., no inverse exists)
        // This might involve returning an error or a default value
    } else {
        double inv_det = 1.0 / det;
        point.x = inv_det * (camera->dir.y * relative_point.x + camera->dir.x * relative_point.y);
        point.y = inv_det * (camera->dir.y * relative_point.y - camera->dir.x * relative_point.x);
        point.z = relative_point.z; // Assuming z and w remain unchanged
        point.w = relative_point.w;
    }

    // Add the camera position back to the point
    point.vec = point.vec + camera->pos.vec;

    return point;
}


// void put_pixel_centered(t_3d_render *render, t_canvas *canvas, double x, double y, uint32_t color)
// {

// 	x *= 10;
// 	y *= 10;

// 	x += render->width/ 2;
// 	y += render->height/ 10;
// 	int offset = ((int)y) * canvas->size.x + x;


// 	if (x < 0 || x >= render->width || y < 0 || y >= render->height)
// 		return ;

// 	// printf("world pos %f %f\n", x, y);

// 	render->buffers.color[offset].d = color;

// }

void draw_wall_texture(
	t_3d_render *render,
	t_segment_d *wall,
	double left,
	double right
)
{
	t_texture_mapping_data data;

	data.texture_width = texture_get_frame(wall->data.data.wall.texture.texture)->size.x;

	invert_if_needed(render, wall);

	data.texture_tiling_factor_x = calc_wall_texture_repeat_factor_x(wall);
	data.texture_tiling_factor_y = calc_wall_texture_repeat_factor_y(wall);

	data.relative_segment = transform_camera_relative_segment(*wall, render->camera);
	data.clipped_relative_segment = data.relative_segment;
	if (relative_segment_clip_front(&data.clipped_relative_segment))
		return ;

	calc_u(&data);

	t_segment_d projected_top;
	t_segment_d projected_bot;
	double coef_top;
	double coef_bot;
	if (project_wall(render, wall, &projected_top, &projected_bot))
		return ;

	coef_bot = calc_segment_coef(&projected_bot);
	coef_top = calc_segment_coef(&projected_top);
	left = fmax(projected_top.point_a.x, left);
	right = fmin(projected_top.point_b.x, right);
	projected_top.point_a.y += coef_top * (left - projected_top.point_a.x);
	projected_top.point_b.y += coef_top * (right - projected_top.point_b.x);
	projected_bot.point_a.y += coef_bot * (left - projected_bot.point_a.x);
	projected_bot.point_b.y += coef_bot * (right - projected_bot.point_b.x);
	double x = left;
	double top = projected_top.point_a.y;
	double bot = projected_bot.point_a.y;

	t_vector4d normal;

	  //  data.clipped_relative_segment.point_b.vec - data.clipped_relative_segment.point_a.vec;
	normal.vec = wall->point_b.vec - wall->point_a.vec;
	normal = (t_vector4d){{-normal.y, normal.x, 0, 0}};
	double size = sqrt(normal.x * normal.x + normal.y * normal.y);
	normal.x /= size;
	normal.y /= size;


	while (x < right)
	{
		double alpha = (x - projected_top.point_a.x) / (projected_top.point_b.x - projected_top.point_a.x);
		double one_over_z = (1 - alpha) / data.clipped_relative_segment.point_a.y + alpha /  data.clipped_relative_segment.point_b.y;
		double relative_x = (((1 - alpha) * (data.clipped_relative_segment.point_a.x / data.clipped_relative_segment.point_a.y) + alpha * (data.clipped_relative_segment.point_b.x / data.clipped_relative_segment.point_b.y))) / one_over_z;
		double relative_y = 1 / one_over_z;
		t_vector4d world_pos = (t_vector4d){{relative_x, relative_y, 0, 0}};
		world_pos = reverse_transform_camera_relative_point(world_pos, render->camera);


		double txtx = (((1 - alpha) * (data.u0 / data.clipped_relative_segment.point_a.y) + alpha * (data.u1 / data.clipped_relative_segment.point_b.y))) / one_over_z;
		txtx = fmin(fmod(txtx, data.texture_width), data.texture_width);
		draw_vertical_line_tiled(render,
			texture_get_frame(wall->data.data.wall.texture.texture), txtx,
			x, top, bot, data.texture_tiling_factor_y, relative_y, &normal, &world_pos, &wall->data.ceil, &wall->data.floor);
		// put_pixel_centered(render, render->canvas, world_pos.x, world_pos.y, 0xFFFFFFFF);
		top += coef_top;
		bot += coef_bot;
		x++;
	}
	// put_pixel_centered(render, render->canvas, data.clipped_relative_segment.point_a.x, data.clipped_relative_segment.point_a.y, 0xFFFFFFFF);
	// put_pixel_centered(render, render->canvas, data.clipped_relative_segment.point_b.x, data.clipped_relative_segment.point_b.y, 0xFFFFFFFF);
}

