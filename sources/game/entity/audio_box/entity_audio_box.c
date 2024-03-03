/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_audio_box.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:03:25 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/03 06:50:17 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "structures.h"

void	entity_audio_box_update(t_entity *self, t_game_data *game_data)
{
	(void)game_data;
	ALfloat sourcePos[] = {self->physics.pos.x, self->physics.pos.y, 0};
	t_entity_audio_box_data	*const data = self->data;
	alSourcefv(data->audio_source, AL_POSITION, sourcePos);
}

void	entity_audio_box_destroy(t_entity *self, t_game_data *game_data)
{
	(void)game_data;
	free(self->data);
}

static void	_init_audio_box_data(
	t_entity *self,
	t_spawn spawn,
	t_game_data *game_data)
{
	t_entity_audio_box_data	*const data = self->data;

	self->physics.pos = spawn.pos;
	self->physics.dir = spawn.dir;
	alGenSources(1, &data->audio_source);
	alSourcei(data->audio_source, AL_BUFFER, game_data->audio_buffers[AMBIANT_DRONE_SOUND]);
	ALfloat sourcePos[] = {self->physics.pos.x, self->physics.pos.y, self->physics.pos.z};
	alSourcefv(data->audio_source, AL_POSITION, sourcePos);
	alSourcei(data->audio_source, AL_LOOPING, 1);
	alSourcePlay(data->audio_source);
}

void	entity_audio_box_draw(t_entity *self, t_game_data *game_data);

t_entity	*entity_audio_box_spawn(t_game_data *game_data, t_spawn	spawn)
{
	t_entity	*self;

	self = entity_default_spawn(game_data);
	if (self == NULL)
		return (NULL);
	self->type = ENTITY_AUDIO_BOX;
	self->destroy = entity_audio_box_destroy;
	self->update = entity_audio_box_update;
	self->data = ft_calloc(1, sizeof(t_entity_audio_box_data));
	if (self->data == NULL)
	{
		return (NULL);
	}
	_init_audio_box_data(self, spawn, game_data);
	return (self);
}
