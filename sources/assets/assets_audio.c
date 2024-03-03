/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_audio.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:19:00 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/03 02:40:45 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "assets.h"
#include "ressources_managers.h"
#include <AL/al.h>
#include <AL/alc.h>

static const char	*g_audio_asset_paths[AUDIO_ASSET_COUNT] = {
[PENGUIN_WALK_SOUND] = "./assets/audio/penguin/test.wav",
};


ALuint	load_audio_asset(const char *filename); // Déclaration de la fonction de chargement

int	load_audio_assets(ALuint *audio_buffers)
{
	int	index;

	index = 0;
	while (index < AUDIO_ASSET_COUNT)
	{
		audio_buffers[index] = load_audio_asset(g_audio_asset_paths[index]);
		if (audio_buffers[index] == 0)
			return (printf("Error\nCould not load audio asset %s\n", g_audio_asset_paths[index]), 1);
		index++;
	}
	return (0);
}

void	destroy_audio_asset(ALuint *audio_buffers)
{
	alDeleteBuffers(1, audio_buffers);
}

int	initialize_and_preload_audio_assets(t_cub *data)
{
	if (load_audio_assets(data->audio_buffers))
		return (1);
	return (0);
}

void	destroy_audio_assets(t_cub *data)
{
	int	index;

	index = 0;
	while (index < AUDIO_ASSET_COUNT)
	{
		destroy_audio_asset(&data->audio_buffers[index]);
		index++;
	}
}
