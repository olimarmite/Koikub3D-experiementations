/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 03:24:57 by motero            #+#    #+#             */
/*   Updated: 2023/10/20 01:56:40 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <stdint.h>
# include <stdio.h>
# include "libft.h"
# include "get_next_line.h"
# include "colors.h"
# include "mlx_int.h"

/*############################################################################*/
/*                              STRUCTURES                                    */
/*############################################################################*/

/* Vector structure for 2D float vector
** Vector structure for 2D unsigned int vector
** Vector structure for 2D int vectors
** causes the compiler to set the mode for foo,
** to be 16 bytes, divided into int sized units.
** SIMD vectorrization
** https://users.ece.cmu.edu/~franzf/teaching/slides-18-645-simd.pdf
**
*/

enum e_action { a_move_up, a_move_down, a_move_left, a_move_right, a_turn_left, a_turn_right, a_total_actions };

typedef float			t_vector_f		__attribute__((vector_size (8)));
typedef unsigned int	t_vector_u		__attribute__((vector_size (8)));
typedef int				t_vector_i		__attribute__((vector_size (8)));

typedef int				t_v2i			__attribute__((vector_size (2 * sizeof(int))));
typedef double			t_v2d			__attribute__((vector_size (2 * sizeof(double))));
typedef u_int8_t		t_vector_color	__attribute__((vector_size (4 * sizeof(u_int8_t))));
typedef double			t_v4d 			__attribute__((vector_size(4 * sizeof(double))));

typedef union u_vector4d
{
	t_v4d	vec;
	struct {
		double	x;
		double	y;
		double	z;
		double	w;
	};
}	t_vector4d;

typedef union u_point2d
{
	t_v2d	vec;
	struct {
		double	x;
		double	y;
	};
}	t_point2d;

/* bpp = bits per pixel */
typedef struct s_img_data
{
	void		*mlx_img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	t_vector_i	size;
}	t_img_data;

/* Parsing structure for CUB3D stocking information from .cub file
** a wolrd map in a 2D int array of MAPWIDTH * MAPHEIGHT
** a array  of size 4 of sprites using t_image from the mlx
** in the order from indx 0 to 3 : N, S, E, W
** a uint_32_t for the color of the floor
** a uint_32_t for the color of the ceiling
*/

typedef union u_color
{
	t_vector_color	rgb_color;
	u_int32_t		d;
	struct
	{
		u_int8_t	a;
		u_int8_t	r;
		u_int8_t	g;
		u_int8_t	b;
	};
}	t_color;

typedef struct s_player
{
	t_vector4d	pos;
	t_vector4d	dir;
	t_vector_f	plane;
}				t_player;

typedef struct s_dda
{
	t_vector_f	ray_dir;
	t_vector_f	side_dist;
	t_vector_f	delta_dist;
	t_vector_i	map;
	t_vector_f	pos;
	t_vector_i	step;
	uint32_t	color;
	float		perp_wall_dist;
	float		tex_pos;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			hit;
	int			side;
	int			tex_y;
	int			x;
	t_vector_i	mouse;
}				t_dda;

typedef struct s_inputs
{
	int		action_states[a_total_actions];
}			t_inputs;

typedef struct s_cub
{
	t_img_data			texture[4];
	t_img_data			screen;
	uint32_t			floor;
	uint32_t			celling;
	t_player			player;
	t_dda				dda;
	size_t				mapwidth;
	size_t				mapheight;
	t_img				img;
	void				*mlx_ptr;
	void				*win_ptr;
	char				**map;
	int					update;
	t_inputs			inputs;
	int					is_drawing;
	t_list				*canvas_list;
}				t_cub;

typedef struct s_data
{
	char		**textures;
	char		**colors;
	char		***map;
}				t_data;

typedef union u_point2i
{
	t_v2i	vec;
	struct {
		int	x;
		int	y;
	};
}	t_point2i;


typedef enum e_segment_type
{
	WALL,
	PORTAL
}	t_segment_type;

typedef struct s_wall_data
{
	int		size;
	double	height;
	double	floor_height;
}	t_wall_data;

typedef struct s_portal_data
{
	int		size;
	int		id;
	int		*destinations_id;
	void	*destination;
}	t_portal_data;

typedef union u_wall_portal_data
{
	t_wall_data		wall;
	t_portal_data	portal;
}	t_wall_portal_data;

typedef union s_segment_data
{
	t_segment_type		type;
	t_wall_portal_data	data;
}	t_segment_data;

typedef struct s_segment_i
{
	t_point2i		point_a;
	t_point2i		point_b;
	t_segment_data	data;
}	t_segment_i;

typedef struct s_segment_d
{
	t_vector4d		point_a;
	t_vector4d		point_b;
	t_segment_data	data;
}	t_segment_d;

typedef enum e_direction{
	NONE = 0,
	RIGHT = 1,
	DOWN = 2,
	DIAGONAL_RIGHT = 4,
	DIAGONAL_LEFT = 8
}	t_direction;

typedef struct s_edge_exploration_context{
	t_vector4d		coord;
	t_vector4d		delta;
	t_vector4d		current_coord;
	t_segment_d		segment;
	char			**map;
	t_direction		direction;
	t_direction		**visited;
}	t_edge_exploration_context;

typedef struct s_tree_node
{
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	void				*data;
}	t_tree_node;

#endif
