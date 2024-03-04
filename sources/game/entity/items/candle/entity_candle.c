/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_candle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:03:25 by olimarti          #+#    #+#             */
/*   Updated: 2024/02/20 23:21:16 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "render_3D.h"
#include "structures.h"
#include "maths_utils.h"
#include "matrix.h"
#include "settings.h"

void	entity_candle_draw(t_entity *self, t_game_data *game_data)
{
	t_entity_candle_data	*data;
	t_billboard				billboard;

	data = self->data;
	billboard.pos = self->physics.pos;
	billboard.dir = self->physics.dir;
	billboard.right = self->physics.right;
	billboard.size = (t_vector4d){{1, 1, 0, 0}};
	billboard.texture.texture
		= &game_data->map_data.texture_manager
		->textures[TEXTURE_CANDLE];
	billboard.texture.offset = 0;
	draw_billboard(&game_data->game_view_render, &billboard);
}


void	entity_candle_destroy(t_entity *self, t_game_data *game_data)
{
	const t_entity_candle_data	*data = self->data;

	if (self->data)
	{
		if (data->light_id != -1)
			light_destroy(&game_data->game_view_render, data->light_id);
	}
	free(self->data);
}

/*
	self_data->light_intensity = 0.5;
	self_data->flicker_duration = 10;
	self_data->flicker_duration_variance = 0.1;
	self_data->flicker_interval = 1000;
	self_data->flicker_interval_variance = 0.2;
	self_data->flicker_intensity_variance = 0.5;*/
static int	_init_candle_data(
		t_game_data *game_data,
		t_entity *self,
		t_entity_candle_data *self_data,
		t_spawn spawn)
{
	t_light	*light;

	self_data->light_intensity = 0.2;
	self_data->flicker_duration = 10;
	self_data->flicker_duration_variance = 0.1;
	self_data->flicker_interval = 15;
	self_data->flicker_interval_variance = 1;
	self_data->flicker_intensity_variance = 0.2;
	self_data->current_interval_duration = self_data->flicker_interval;
	self->physics.pos = spawn.pos;
	self->physics.pos.z = 2;
	self->physics.dir = spawn.dir;
	self->physics.right = (t_vector4d){{spawn.dir.y, -spawn.dir.x, 0, 0}};
	self_data->light_id = light_spawn_default(&game_data->game_view_render);
	if (self_data->light_id == -1)
		return (1);
	light = sparse_array_get(game_data->game_view_render.lights_data.lights,
			self_data->light_id);
	light->type = POINT_LIGHT;
	light->pos = self->physics.pos;
	light->color = (t_color){.r = 4, .g = 3, .b = 0};
	light->dir = self->physics.dir;
	light->intensity = self_data->light_intensity;
	light->show_lens_flare = true;
	light->use_raycasting = true;
	return (0);
}

t_entity	*entity_candle_spawn(t_game_data *game_data, t_spawn spawn)
{
	t_entity	*self;

	self = entity_default_spawn(game_data);
	if (self == NULL)
		return (NULL);
	self->type = ENTITY_CANDLE;
	self->destroy = entity_candle_destroy;
	self->update = entity_candle_update;
	self->draw = entity_candle_draw;
	self->data = ft_calloc(1, sizeof(t_entity_candle_data));
	if (self->data == NULL)
	{
		return (NULL);
	}
	if (_init_candle_data(game_data, self, self->data, spawn))
	{
		return (NULL);
	}
	return (self);
}
