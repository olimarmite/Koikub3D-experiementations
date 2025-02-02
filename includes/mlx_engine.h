/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_engine.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:27 by motero            #+#    #+#             */
/*   Updated: 2024/03/03 10:21:13 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_ENGINE_H
# define MLX_ENGINE_H

# include "cub3D.h"
# include "mlx.h"
//# include "structures.h"
# include "draw_utils.h"

/*############################################################################*/
/*                              BUTTONS DEFINITION                            */
/*############################################################################*/

# define UP_KEY 65362
# define DOWN_KEY 65364
# define LEFT_KEY 65361
# define RIGHT_KEY 65363
# define MINUS_PAD 65453
# define PLUS_PAD 65451
# define ONE_PAD 65436
# define TWO_PAD 65433
# define THREE_PAD 65435
# define FOUR_PAD 65430
# define FIVE_PAD 65437
# define SIX_PAD 65432
# define MINUS_KEY 45
# define PLUS_KEY 61
# define BRACE_L_KEY 91
# define BRACE_R_KEY 93
// # define XK_d 100
// # define C_KEY 99
// # define Z_KEY 122
// # define Q_KEY 113
// # define S_KEY 115
// # define XK_a 97
// # define W_KEY 119

typedef struct s_key_action_map
{
	int				key_id;
	enum e_action	action;
}	t_key_action_map;

static const t_key_action_map	g_key_to_action[] = {
{XK_w, a_move_forward},
{XK_s, a_move_backward},
{XK_a, a_move_left},
{XK_d, a_move_right},
{XK_Shift_L, a_move_down},
{XK_space, a_move_up},
{XK_Left, a_turn_left},
{XK_Right, a_turn_right},
{XK_Control_L, a_run},
{XK_equal, a_increase_sector_ceil},
{XK_minus, a_decrease_sector_ceil},
{XK_o, a_increase_sector_floor},
{XK_p, a_decrease_sector_floor},
{XK_Shift_R, a_interact},
{-1, -1},
};

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1020

# ifndef FOV
#  define FOV 90
# endif

/*############################################################################*/
/*                              MLX_ENGINE FUNCTIONS                          */
/*############################################################################*/
int				ft_mlx_initialize_pointers(t_cub *data);
int				ft_mlx_create_window_and_image(t_cub *data);

/*############################################################################*/
/*                              MLX_ENGINE HOOKS                          */
/*############################################################################*/

void			ft_mlx_engine(t_cub *data);
int				ft_destroy_window(t_cub *data);
int				ft_handle_keyrelease(int keysym, t_cub *data);
int				ft_handle_keypress(int keysym, t_cub *data);
int				ft_handle_boutonpress(int buttonsym, int x, int y, t_cub *data);
int				ft_handle_mousemotion(int x, int y, t_cub *data);

/*############################################################################*/
/*                              KEYPRESS EVENTS                               */
/*############################################################################*/

void			ft_destroy_window_button(int keysym, t_cub *data);
void			ft_movements_keys(int keysym, t_cub *data, int state);

/*############################################################################*/
/*                              MAP EDITOR EVENTS                             */
/*############################################################################*/
int				map_canvas_handle_keypress(
					int keysym,
					void *self,
					t_cub *data);
int				map_canvas_handle_boutonpress(
					int buttonsym,
					t_point2i mouse_pos,
					void *self,
					t_cub *data);
int				map_canvas_handle_zoom(
					int zoom_direction,
					void *map_canvas,
					t_cub *data,
					t_point2i mouse_pos);
int				map_editor_render(void *self, t_cub *data);
int				map_editor_handle_rotation(int keysim, t_cub *data,
					void *map_canvas);

/*############################################################################*/
/*                             GAME EVENTS                                    */
/*############################################################################*/

int				ft_handle_game_keypress(int keysym, void *canvas, t_cub *data);
int				ft_handle_game_keyrelease(int keysym, void *canvas, t_cub *data);
int				menu_to_game_handle_boutonpress(
					int buttonsym,
					t_point2i mouse_pos,
					void *self,
					t_cub *data);
/*############################################################################*/
/*                             BUTTON EVENTS                               */
/*############################################################################*/
int				menu_to_edit_map_handle_boutonpress(
					int buttonsym,
					t_point2i mouse_pos,
					void *self,
					t_cub *data);
int				button_render(void *self, t_cub *data);

/*############################################################################*/
/*                             CANVAS EVENTS                                  */
/*############################################################################*/
void			*detect_clicked_canvas(t_cub *data, t_point2i click_position);


#endif
