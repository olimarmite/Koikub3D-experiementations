/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_penguin_logic.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 02:46:54 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/03 22:57:54 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "maths_utils.h"
#include "matrix.h"
#include "settings.h"
#include "structures.h"

static void	normalize_vector_3d(t_vector4d *vec)
{
	double	reverse_lenght;
	double	lenght;

	lenght = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	if (lenght == 0)
		return ;
	reverse_lenght = 1 / lenght;
	vec->vec[0] *= reverse_lenght;
	vec->vec[1] *= reverse_lenght;
	vec->vec[2] *= reverse_lenght;
}

t_vector4d	_get_penguin_world_acceleration(
	t_entity *self,
	t_entity_penguin_data *self_data,
	t_game_data *game_data)
{
	t_vector4d	acceleration;

	(void)self_data;
	acceleration.x = game_data->state.player->physics.pos.x
		- self->physics.pos.x;
	acceleration.y = game_data->state.player->physics.pos.y
		- self->physics.pos.y;
	acceleration.z = 0;
	if (acceleration.x * acceleration.x + acceleration.y * acceleration.y < 10)
	{
		acceleration = (t_vector4d){{0, 0, 0, 0}};
		return (acceleration);
	}
	normalize_vector_3d(&acceleration);
	acceleration.vec *= DEFAULT_PLAYER_ACCELERATION / 3;
	return (acceleration);
}

void	_update_penguin_direction(
	t_entity *self,
	t_entity_penguin_data *self_data,
	t_game_data *game_data)
{
	t_vector4d	direction;

	(void)self_data;
	direction.x = game_data->state.player->physics.pos.x - self->physics.pos.x;
	direction.y = game_data->state.player->physics.pos.y - self->physics.pos.y;
	direction.z = 0;
	normalize_vector_3d(&direction);
	self->physics.dir.x = direction.x * 0.1 + self->physics.dir.x * 0.90;
	self->physics.dir.y = direction.y * 0.1 + self->physics.dir.y * 0.90;
	self->physics.right = self->physics.dir;
	self->physics.right.x = self->physics.dir.y;
	self->physics.right.y = -self->physics.dir.x;
}

static void playRandomFootstep(t_entity *self) {
	t_entity_player_data *const data = self->data;
    int numFootsteps = sizeof(data->audio_footstep_source) / sizeof(data->audio_footstep_source[0]);
    int nextSound;

	if (self->physics.acceleration.x == 0
		&& self->physics.acceleration.y == 0)
	{
		data->last_footstep_sound_time = 0;
		return;
	}

	data->footstep_interval = 10;

	// alSourcefv(data->audio_source, AL_POSITION, sourcePos);


	if (data->last_footstep_sound_time > 0)
	{
		data->last_footstep_sound_time -= 1;
		return;
	}

    do {
        nextSound = rand() % numFootsteps; // Randomly select the next sound
    } while (nextSound == data->last_footstep_sound); // Make sure the next sound is different from the last played sound

    // Play the selected sound
	ALfloat sourcePos[] = {self->physics.pos.x, self->physics.pos.y, self->physics.pos.z};
	alSourcefv(data->audio_footstep_source[nextSound], AL_POSITION, sourcePos);
	// alSourcef(data->audio_footstep_source[nextSound], AL_ROLLOFF_FACTOR, 0.0f);
    alSourcePlay(data->audio_footstep_source[nextSound]);

    // Update the last played sound index
    data->last_footstep_sound = nextSound;
	data->last_footstep_sound_time = data->footstep_interval;
}


void	entity_penguin_update_movements(t_entity *self, t_game_data *game_data)
{
	t_entity_penguin_data	*data;
	t_vector4d				world_space_acceleration;

	data = self->data;
	world_space_acceleration
		= _get_penguin_world_acceleration(self, data, game_data);
	_update_penguin_direction(self, data, game_data);
	self->physics.acceleration.vec = world_space_acceleration.vec;
	playRandomFootstep(self);
}
