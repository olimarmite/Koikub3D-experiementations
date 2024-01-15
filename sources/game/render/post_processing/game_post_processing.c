
#include "draw_utils.h"
#include "structures.h"
#include "render_3D.h"

void	game_post_process_frame(t_3d_render *render)
{
	const int max_offset = render->canvas->size.x * render->canvas->size.y;
	int	i;

	i = 0;
	while (i < max_offset)
	{
		// if (render->z_buffer[i] >= 1)
		// {
		// 	render->canvas->pixels[i].a /= render->z_buffer[i];
		// 	render->canvas->pixels[i].r /= render->z_buffer[i];
		// 	render->canvas->pixels[i].g /= render->z_buffer[i];
		// 	render->canvas->pixels[i].b /= render->z_buffer[i];
		// }
		render->canvas->pixels[i] = shader_torch(render->canvas->pixels[i], i,
			render->canvas->size.x, render->canvas->size.y, render);
		++i;
	}
}
