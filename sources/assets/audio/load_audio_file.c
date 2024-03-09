/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_audio_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 06:09:05 by olimarti          #+#    #+#             */
/*   Updated: 2024/02/28 05:45:45 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>

static ALenum	get_format(int channels, int format)
{
	if (channels == 1)
	{
		if ((format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_16)
			return (AL_FORMAT_MONO16);
		return (AL_FORMAT_MONO8);

	}
	else if (channels == 2)
	{
		if ((format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_16)
			return (AL_FORMAT_STEREO16);
		return (AL_FORMAT_STEREO8);
	}
	return (-1);
}

static void	read_audio_data(
	SNDFILE *file,
	SF_INFO sfinfo,
	ALshort **buffer_data,
	ALsizei *data_size)
{
	ALsizei	num_samples;

	num_samples = sfinfo.frames * sfinfo.channels;
	*data_size = num_samples * (sfinfo.format & SF_FORMAT_SUBMASK);
	*buffer_data = malloc(*data_size);
	if (!*buffer_data)
	{
		printf("Failed to allocate memory for audio data\n");
		return ;
	}
	if (sf_read_short(file, *buffer_data, num_samples) < num_samples)
	{
		printf("Failed to read audio data\n");
		free(*buffer_data);
		*buffer_data = NULL;
	}
}

ALuint	create_buffer_from_file(SNDFILE *file, SF_INFO sfinfo)
{
	ALshort	*buffer_data;
	ALenum	format;
	ALuint	buffer;
	ALsizei	data_size;

	read_audio_data(file, sfinfo, &buffer_data, &data_size);
	if (!buffer_data)
		return (0);
	format = get_format(sfinfo.channels, sfinfo.format);
	if (format == -1)
		return (0);
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, buffer_data, data_size, sfinfo.samplerate);
	free(buffer_data);
	return (buffer);
}

ALuint	load_audio_asset(const char *filename)
{
	SNDFILE	*file;
	SF_INFO	sfinfo;
	ALuint	buffer;

	file = sf_open(filename, SFM_READ, &sfinfo);
	if (!file)
	{
		printf("Failed to open audio file %s\n", filename);
		return (0);
	}
	buffer = create_buffer_from_file(file, sfinfo);
	sf_close(file);
	return (buffer);
}
