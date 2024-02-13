/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_torch_logic.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 02:02:29 by olimarti          #+#    #+#             */
/*   Updated: 2024/02/13 02:44:59 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "render_3D.h"
#include "structures.h"
#include "maths_utils.h"

void	_torch_flicker_effect(
	t_entity_torch_data *data,
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

void	_update_position(
	t_entity_torch_data *data,
	t_light *light,
	t_3d_render *render)
{
	data->pos.x = (render->camera->pos.x - render->camera->dir.y * 0.15) * 0.9
		+ data->pos.x * 0.1;
	data->pos.y = (render->camera->pos.y + render->camera->dir.x * 0.15) * 0.9
		+ data->pos.y * 0.1;
	data->pos.z = (render->camera->pos.z + 0.1) * 0.5 + data->pos.z * 0.5;
	data->dir.x = -(render->camera->dir.x) * 0.2 + data->dir.x * 0.8;
	data->dir.y = -(render->camera->dir.y) * 0.2 + data->dir.y * 0.8;
	light->pos = data->pos;
	light->dir = data->dir;
}

void	_update_flickering_params(
	t_entity_torch_data *data,
	t_game_data *game_data)
{
	double	dist;

	dist = sqrt(pow(data->pos.x - game_data->map_data.player_spawn.pos.x, 2)
			+ pow(data->pos.y - game_data->map_data.player_spawn.pos.y, 2));
	data->flicker_interval = 1000 / (dist + 1);
	if (data->flicker_interval
		* (1 + data->flicker_interval_variance)
		<= data->current_interval_duration)
	{
		data->current_interval_duration = data->flicker_interval;
	}
}

void	entity_torch_update(t_entity *self, t_game_data *game_data)
{
	t_entity_torch_data	*data;
	t_3d_render			*render;
	t_light				*light;

	data = self->data;
	render = &game_data->game_view_render;
	light = sparse_array_get(render->lights_data.lights,
			data->light_id);

	_update_position(data, light, render);
	_update_flickering_params(data, game_data);
	_torch_flicker_effect(data, light, game_data);
}
