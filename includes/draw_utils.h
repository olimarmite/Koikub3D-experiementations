/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 16:53:46 by olimarti          #+#    #+#             */
/*   Updated: 2023/10/16 23:10:14 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_UTILS_H
# define DRAW_UTILS_H

# include <stdbool.h>
# include "mlx.h"
# include "structures.h"
# include "colors.h"
# include "matrix.h"
# include  "list_fonctions.h"

# define MAP_CANVAS_SIZE_X 1920
# define MAP_CANVAS_SIZE_Y 1072
# define UI_CANVAS_SIZE_X 1000
# define UI_CANVAS_SIZE_Y 1000
# define FINAL_CANVAS_SIZE_X 1920
# define FINAL_CANVAS_SIZE_Y 1072

# define MAP_Z_INDEX 3
# define UI_Z_INDEX 5
# define FINAL_Z_INDEX 99
# define FINAL_TEMP_Z_INDEX 99
# define END_MARKER_Z_INDEX 0

/*############################################################################*/
/*                              GEOMETRY STRUTURE                             */
/*############################################################################*/

typedef enum e_shape_type
{
	LINE,
	RECTANGLE,
	CIRCLE,
	TRIANGLE
}	t_shape_type;

// LINE
typedef struct s_line_params {
	t_point2d	start;
	union
	{
		t_point2d	end;
		double		length;
	};
}	t_line_params;

// Rectangle
typedef struct s_rectangle_params {
	t_point2d	top_left;
	t_point2d	top_right;
	t_point2d	bottom_left;
	t_point2d	bottom_right;
}	t_rectangle_params;

// Circle
typedef struct s_circle_params {
	t_point2d	center;
	int			radius;
}	t_circle_params;

// Triangle
typedef struct s_triangle_params {
	t_point2d	pt1;
	t_point2d	pt2;
	t_point2d	pt3;
}	t_triangle_params;

typedef union u_shape
{
	t_line_params		line;
	t_rectangle_params	rect;
	t_circle_params		circle;
	t_triangle_params	triangle;	
}	t_shape;

typedef struct s_shape_params {
	t_img_data		*img;
	t_shape_type	shape_type;
	t_color			color;
	t_shape			shape;
}	t_shape_params;

/*############################################################################*/
/*                              CANVAS STRUCTURES                             */
/*############################################################################*/

typedef enum e_canvas_type
{
	MAP,
	UI,
	FINAL,
	FINAL_TEMP,
	END_MARKER
}	t_canvas_type;

typedef struct s_bounds {
	t_point2i	top;
	t_point2i	bottom;
}	t_bounds;

typedef struct s_canvas {
	t_canvas_type	type;
	t_matrix3x3		transformation_matrix;
	t_point2i		size;
	t_point2d		last_point;
	t_point2d		scale;
	t_point2d		inv_scale;
	t_color			*pixels;
	t_list			*matrix_operations;
	t_list			*segments;
	t_bounds		bounds;
	t_color			transparency_key;
	bool			is_dynamic;
	int				z_index;
}	t_canvas;

typedef struct s_canvas_init_entry {
	t_point2i		size;
	t_canvas_type	type;
	int				z_index;
	t_point2i		position;
	t_bounds		bounds;
	bool			stack;
	bool			is_dynamic;
}	t_canvas_init_entry;

/*############################################################################*/
/*                              CANVAS INITIALIZATION                         */
/*############################################################################*/

t_canvas					*initialize_single_canvas(t_point2i size,
								t_canvas_type type);
t_list						*initialize_canvas_and_add_to_list(t_point2i size,
								t_canvas_type type,
								t_list **canvas_list);
t_list						*initialize_canvas_list(void);
void						free_canvas(t_canvas *canvas);
void						free_canvas_list(t_list *canvas_list);
t_canvas					*get_canvas_from_list(t_list *canvas_list,
								t_canvas_type type);
const t_canvas_init_entry	*get_canvas_init_table(void);
void						set_canvas_bounds(t_canvas_init_entry *entry,
								t_point2i *currentPos,
								int *currentRowHeight,
								int maxWidth);

/*############################################################################*/
/*                              MAP CANVAS OPERATIONS                         */
/*############################################################################*/

int							add_segment_to_map(t_canvas *canvas,
								t_segment_d segment);
int							erase_line_from_map(t_canvas *canvas,
								t_segment_d segment);
int							apply_transformation_to_map(t_canvas *canvas,
								t_matrix3x3 transform);

/*############################################################################*/
/*                              DRAWING CANVAS OPERATIONS                     */
/*############################################################################*/

void						put_pixel_on_canvas(t_canvas *canvas,
								t_point2i coord,
								t_color color);
void						put_pixel_on_virtual_canvas(t_canvas *canvas,
								t_point2i coord,
								t_color color);
int							draw_line_on_map(t_canvas *canvas,
								t_point2i start,
								t_point2i end,
								t_color color);
void						start_drawing(t_canvas *canvas,
								t_point2i start_point);
void						update_drawing(t_canvas *canvas,
								t_point2i current_point,
								t_color color);
void						end_drawing(t_canvas *canvas,
								t_point2i end_point,
								t_color color);

/*############################################################################*/
/*                              MULTI-BUFFER CANVAS                           */
/*############################################################################*/

t_canvas					*get_canvas(t_list *canvas_list,
								t_canvas_type type);

/*############################################################################*/
/*                              UI CANVAS OPERATIONS                          */
/*############################################################################*/

int							draw_UI_elements(t_canvas *ui_canvas);
void						copy_canvas_to_temp(t_list *canvas_list);
void						copy_temp_to_screen(t_list *canvas_list,
								t_img_data *img);
void						copy_temp_to_canvas(t_list *canvas_list);
t_point2i					screen_to_canvas(t_point2i screen_point,
								t_canvas *canvas);

/*############################################################################*/
/*                              FUSING CANVASES                               */
/*############################################################################*/

int							fuse_canvases(t_canvas **array_of_canvases);

/*############################################################################*/
/*                              MLX CONVERSION                                */
/*############################################################################*/

void						canvas_to_mlx_image(t_img_data screen,
								t_canvas *final_canvas);

/*############################################################################*/
/*                              DRAW FUNCTIONS                                */
/*############################################################################*/

void						img_pix_put(t_img_data *img, int x, int y,
								int color);
void						draw_segment(
								t_img_data *img,
								t_segment_d const *const segment,
								int color
								);
void						draw_one_line(
								t_img_data *img,
								t_line_params const *const segment,
								t_color color
								);

/*############################################################################*/
/*                              DRAW SHAPES                                */
/*############################################################################*/
int							draw_line(t_shape_params *params);
int							draw_rectangle(t_shape_params *params);
void						draw_circle_points(t_img_data *img,
								t_point2i center,
								t_point2i point,
								t_color color);
int							draw_circle(t_shape_params *params);

/*############################################################################*/
/*                              ALIGNMENT METHODS                             */
/*############################################################################*/
void						*aligned_malloc(size_t size, size_t alignment);
void						aligned_free(void *ptr);
void						*aligned_calloc(size_t nmemb,
								size_t size,
								size_t alignment);

/*############################################################################*/
/*                              ALIGNMENT METHODS                             */
/*############################################################################*/
int							map_visualizer_draw(t_cub *data);
void						redraw_scene(t_cub *data, t_canvas *canvas);

#endif
