/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_penguin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:03:25 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/03 09:34:27 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "structures.h"

void	entity_penguin_update(t_entity *self, t_game_data *game_data)
{
	(void)self;
	(void)game_data;
	entity_penguin_update_movements(self, game_data);
	ALfloat sourcePos[] = {self->physics.pos.x, self->physics.pos.y, 0};
	t_entity_penguin_data	*const data = self->data;
	alSourcefv(data->audio_source, AL_POSITION, sourcePos);
}

void	entity_penguin_destroy(t_entity *self, t_game_data *game_data)
{
	(void)game_data;
	free(self->data);
}

static void	_init_penguin_data(
	t_entity *self,
	t_spawn spawn,
	t_game_data *game_data)
{
	t_entity_penguin_data	*const data = self->data;

	self->physics.pos = spawn.pos;
	self->physics.dir = spawn.dir;
	self->physics.velocity = (t_vector4d){{0, 0, 0, 0}};
	self->physics.acceleration = (t_vector4d){{0, 0, 0, 0}};
	self->physics.right = (t_vector4d){{spawn.dir.y, -spawn.dir.x, 0, 0}};
	self->physics.friction = DEFAULT_PLAYER_DECELERATION;
	self->physics.collision_model.type = COLLISION_MODEL_DYNAMIC_CYLINDER;
	self->physics.collision_model.dynamic_cylinder.height
		= DEFAULT_PLAYER_HEIGHT;
	self->physics.collision_model.dynamic_cylinder.radius
		= DEFAULT_PLAYER_RADIUS;
	alGenSources(1, &data->audio_source);
	// alSourcei(data->audio_source, AL_BUFFER, game_data->audio_buffers[PENGUIN_WALK_SOUND]);
	// ALfloat sourcePos[] = {self->physics.pos.x, self->physics.pos.y, self->physics.pos.z};
	// alSourcefv(data->audio_source, AL_POSITION, sourcePos);
	// alSourcePlay(data->audio_source);

	alGenSources(5, data->audio_footstep_source);
	alSourcei(data->audio_footstep_source[0], AL_BUFFER, game_data->audio_buffers[WALK_STONE_SOUND_00]);
	alSourcei(data->audio_footstep_source[1], AL_BUFFER, game_data->audio_buffers[WALK_STONE_SOUND_01]);
	alSourcei(data->audio_footstep_source[2], AL_BUFFER, game_data->audio_buffers[WALK_STONE_SOUND_02]);
	alSourcei(data->audio_footstep_source[3], AL_BUFFER, game_data->audio_buffers[WALK_STONE_SOUND_03]);
	alSourcei(data->audio_footstep_source[4], AL_BUFFER, game_data->audio_buffers[WALK_STONE_SOUND_04]);

}

void	entity_penguin_draw(t_entity *self, t_game_data *game_data);

t_entity	*entity_penguin_spawn(t_game_data *game_data, t_spawn	spawn)
{
	t_entity	*self;

	self = entity_default_spawn(game_data);
	if (self == NULL)
		return (NULL);
	self->type = ENTITY_PENGUIN;
	self->destroy = entity_penguin_destroy;
	self->update = entity_penguin_update;
	self->draw = entity_penguin_draw;
	self->data = ft_calloc(1, sizeof(t_entity_penguin_data));
	if (self->data == NULL)
	{
		return (NULL);
	}
	_init_penguin_data(self, spawn, game_data);
	return (self);
}
