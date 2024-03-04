/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 23:54:17 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/03 11:19:23 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_loop.h"
#include "maths_utils.h"
#include "settings.h"

static double	_dot_product_3d(t_vector4d *vec1, t_vector4d *vec2)
{
	const t_v4d	product = vec1->vec * vec2->vec;

	return (product[0] + product[1] + product[2]);
}

static void	update_effects(t_game_data *data)
{
	t_camera_effects	*effects;

	effects = &data->state.player_camera.effects;
	effects->posterization.enabled = true;
	effects->posterization.levels = 4;
	effects->wave.enabled = true;
	if (effects->wave.enabled)
	{
		effects->wave.amplitude_x = 0.2;
		effects->wave.amplitude_y = 1;
		effects->wave.frequency_x = 0.005;
		effects->wave.frequency_y = 10;
		effects->wave.offset_x += 0.05;
		effects->wave.offset_y += 2;
	}
}

// #define TILT_FACTOR 0.02 // Adjust for tilt intensity
#define BOB_AMPLITUDE 0.02 // Vertical bob amplitude, adjust for effect intensity
#define BOB_FREQUENCY 0.1 // Frequency of the bobbing effect
#define ROTATION_AMPLITUDE 0.00 // Tilt rotation amplitude, adjust for effect intensity

// void update_camera_with_bobbing(YourDataType *data) {

//     // Rest of your camera update logic...
// }



void	game_update_camera(t_game_data *data)
{

	double tilt, speed, bobOffset, rotationTilt;

    // Update camera base position and direction as before
    data->state.player_camera.dir = data->state.player->physics.dir;
    data->state.player_camera.right = data->state.player->physics.right;
    data->state.player_camera.pos = data->state.player->physics.pos;
    data->state.player_camera.velocity = data->state.player->physics.velocity;

    // Calculate tilt and speed for bobbing effects
    tilt = _dot_product_3d(&data->state.player_camera.velocity, &data->state.player_camera.right);
    data->state.player_camera.tilt = tilt * TILT_FACTOR;
    speed = sqrt(data->state.player_camera.velocity.vec[0] * data->state.player_camera.velocity.vec[0] +
                 data->state.player_camera.velocity.vec[1] * data->state.player_camera.velocity.vec[1]);

	// if (speed > 0.001)
		// speed = 0.1;

    // Apply vertical bobbing effect based on player's speed
    bobOffset = BOB_AMPLITUDE * sin((double)data->state.time_since_start * BOB_FREQUENCY);
    data->state.player_camera.pos.z += bobOffset;

    // Apply a slight rotational tilt effect for added realism
    rotationTilt = ROTATION_AMPLITUDE * cos((double)data->state.time_since_start * BOB_FREQUENCY);
    // Assuming you have a mechanism to apply this tilt. This might represent a pitch or roll depending on your camera setup
    data->state.player_camera.tilt += rotationTilt; // Modify this according to how your camera's rotation is handled

	update_effects(data);
	// Définir la position de l'auditeur
	ALfloat listenerPos[] = {data->state.player_camera.pos.x, data->state.player_camera.pos.y, 0};
	alListenerfv(AL_POSITION, listenerPos);

	// Définir la direction de l'auditeur
	ALfloat listenerOri[] = {-data->state.player_camera.dir.x, -data->state.player_camera.dir.y, 0, 0, 0, 1};
	alListenerfv(AL_ORIENTATION, listenerOri);

	// Définir la position de l'auditeur
	// ALfloat listenerPos[] = {0.0f, 0.0f, 0.0f}; // Exemple de position
	// alListenerfv(AL_POSITION, listenerPos);

}
