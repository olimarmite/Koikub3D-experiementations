/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_candle_logic.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 02:02:29 by olimarti          #+#    #+#             */
/*   Updated: 2024/02/21 02:49:59 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "render_3D.h"
#include "structures.h"
#include "maths_utils.h"

static void	_candle_flicker_effect(
	t_entity_candle_data *data,
	t_light *light,
	t_game_data *game_data)
{
	if (game_data->state.time_since_start
		> data->current_interval_duration + data->last_flicker_time)
	{
		data->last_flicker_time = game_data->state.time_since_start;
		data->flicker_remaining_duration = data->flicker_duration
			+ random_double_xorshift32() * data->flicker_duration_variance;
		data->current_interval_duration = data->flicker_interval
			+ data->flicker_interval
			* data->flicker_interval_variance
			* (random_double_xorshift32() - 0.5);
	}
	if (data->flicker_remaining_duration > 0)
	{
		light->intensity = fmin(fmax(0, light->intensity
					+ data->flicker_intensity_variance
					* (random_double_xorshift32() - 0.5)), 1);
		data->flicker_remaining_duration -= game_data->state.delta_time;
		if (data->flicker_remaining_duration <= 0)
		{
			light->intensity = data->light_intensity;
		}
	}
}

static void	_update_position(
	t_entity *self,
	t_light *light,
	t_game_data *game_data)
{
	t_3d_render			*render;

	render = &game_data->game_view_render;
	game_data->state.player->physics.dir;

	t_vector4d		player_pos = game_data->state.player->physics.pos;
	t_vector4d		player_dir = game_data->state.player->physics.dir;

	self->physics.pos.x = (player_pos.x + player_dir.x * 0.7)
		* 0.5 + self->physics.pos.x * 0.5;
	self->physics.pos.y = (player_pos.y + player_dir.y * 0.7)
		* 0.5 + self->physics.pos.y * 0.5;
	self->physics.pos.z = (player_pos.z + 0.3)
		* 0.5 + self->physics.pos.z * 0.5;
	self->physics.dir.x = -(render->camera->dir.x)
		* 1 + self->physics.dir.x * 0.0;
	self->physics.dir.y = -(render->camera->dir.y)
		* 1 + self->physics.dir.y * 0.0;
	self->physics.right = (t_vector4d){{self->physics.dir.y, -self->physics.dir.x, 0, 0}};
	light->pos.vec = self->physics.pos.vec + (t_v4d){0, 0, -0.202, 0};
	light->pos.vec += self->physics.dir.vec * 0.01;
	light->dir = self->physics.dir;
}

static void	_update_flickering_params(
	t_entity *self,
	t_entity_candle_data *data,
	t_game_data *game_data)
{
	double	dist;

	dist = sqrt(pow(self->physics.pos.x
				- game_data->map_data.player_spawn.pos.x, 2)
			+ pow(self->physics.pos.y
				- game_data->map_data.player_spawn.pos.y, 2));
	data->flicker_interval = 1000 / (dist + 1);
	if (data->flicker_interval
		* (1 + data->flicker_interval_variance)
		<= data->current_interval_duration)
	{
		data->current_interval_duration = data->flicker_interval;
	}
}

void	entity_candle_update(t_entity *self, t_game_data *game_data)
{
	t_entity_candle_data	*data;
	t_3d_render			*render;
	t_light				*light;

	data = self->data;
	render = &game_data->game_view_render;
	light = sparse_array_get(render->lights_data.lights,
			data->light_id);
	_update_position(self, light, game_data);
	// _update_flickering_params(self, data, game_data);
	_candle_flicker_effect(data, light, game_data);
}
