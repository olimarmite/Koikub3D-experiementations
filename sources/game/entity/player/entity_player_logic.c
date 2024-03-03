/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_player_logic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:19:23 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/03 09:30:32 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "maths_utils.h"
#include "matrix.h"
#include "settings.h"
#include "structures.h"
#include <AL/al.h>
#include <AL/alc.h>

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

t_vector4d	_get_player_space_acceleration(t_game_data *game_data)
{
	t_vector4d	acceleration;

	acceleration.x = game_data->inputs->action_states[a_move_right]
		- game_data->inputs->action_states[a_move_left];
	acceleration.y = game_data->inputs->action_states[a_move_forward]
		- game_data->inputs->action_states[a_move_backward];
	acceleration.z = game_data->inputs->action_states[a_move_down]
		- game_data->inputs->action_states[a_move_up];
	normalize_vector_3d(&acceleration);
	acceleration.vec *= DEFAULT_PLAYER_ACCELERATION;
	return (acceleration);
}

t_vector4d	_get_player_world_acceleration(
	t_entity *self,
	t_game_data *game_data)
{
	t_vector4d	acceleration;
	t_vector4d	world_space_acceleration;

	acceleration = _get_player_space_acceleration(game_data);
	world_space_acceleration.x = acceleration.x * self->physics.right.x
		+ acceleration.y * self->physics.dir.x;
	world_space_acceleration.y = acceleration.x * self->physics.right.y
		+ acceleration.y * self->physics.dir.y;
	world_space_acceleration.z = acceleration.z;
	return (world_space_acceleration);
}

void	_update_player_direction(
		t_entity *self,
		t_game_data *game_data)
{
	const double	angle_scale = DEFAULT_PLAYER_ROTATION_SPEED;
	double			angle_movement;
	t_matrix3x3		transformation;
	t_point2d		rotated_dir;

	angle_movement = angle_scale
		* (game_data->inputs->action_states[a_turn_right]
			- game_data->inputs->action_states[a_turn_left]);
	transformation = rotation_matrix(angle_movement);
	rotated_dir = matrix_vector_multiply(transformation,
			vector4d_to_point2d(&self->physics.dir));
	self->physics.dir = point2d_to_vector4d(&rotated_dir);
	self->physics.right = self->physics.dir;
	self->physics.right.x = -self->physics.dir.y;
	self->physics.right.y = self->physics.dir.x;
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
    alSourcePlay(data->audio_footstep_source[nextSound]);
    printf("Playing footstep sound: %d\n", nextSound);

    // Update the last played sound index
    data->last_footstep_sound = nextSound;
	data->last_footstep_sound_time = data->footstep_interval;
}

void	entity_player_update_movements(t_entity *self, t_game_data *game_data)
{
	t_entity_player_data	*data;
	t_vector4d				world_space_acceleration;

	data = self->data;
	world_space_acceleration
		= _get_player_world_acceleration(self, game_data);
	self->physics.acceleration = world_space_acceleration;
	_update_player_direction(self, game_data);
	playRandomFootstep(self);
}
