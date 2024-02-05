/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_oriented_textures.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 22:21:30 by olimarti          #+#    #+#             */
/*   Updated: 2024/02/05 01:00:02 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structures.h"
#include <math.h>
#include <stdio.h>

typedef struct s_oriented_texture
{
	int		texture_id;
	char	name[4];
	double	sector_id;
}	t_oriented_texture;

//TODO probably use this to store the textures in the texture manager
static const t_oriented_texture	g_oriented_textures[] = {
{0, "NO", 3},
{1, "EA", 0},
{2, "SO", 1},
{3, "WE", 2},
};

static int	calc_vector_sector(t_vector4d *vec, int sector_count)
{
	double	angle;

	angle = atan2(vec->y, vec->x);
	if (angle < 0)
		angle += M_PI * 2;
	return (sector_count * angle / (2 * M_PI));
}

static t_texture	*get_oriented_texture(
	t_game_data *game_data,
	t_segment_d *segment,
	t_texture_manager *texture_manager
	)
{
	unsigned long	i;
	int				sector_id;
	int				sector_count;

	sector_count = fmin(game_data->map_data.texture_manager->texture_count,
			sizeof(g_oriented_textures) / sizeof(t_oriented_texture));
	sector_id = calc_vector_sector(&segment->data.normal,
			game_data->map_data.texture_manager->texture_count);
	i = 0;
	while (i < sizeof(g_oriented_textures) / sizeof(t_oriented_texture))
	{
		if (g_oriented_textures[i].sector_id == sector_id)
		{
			return (
				&texture_manager->textures[g_oriented_textures[i].texture_id]
			);
		}
		++i;
	}
	return (NULL);
}

// Note: Make sure to calculate the normal before calling this function
void	compute_oriented_textures(
	t_game_data *game_data,
	t_segment_d *segment,
	__attribute_maybe_unused__ t_bsp_tree_node_data *sector)
{
	t_texture	*texture;

	texture = get_oriented_texture(game_data, segment,
			game_data->map_data.texture_manager);
	if (segment->data.type == WALL)
	{
		segment->data.data.wall.texture.texture
			= texture;
	}
	else
	{
		segment->data.data.portal.padding_texture_bottom.texture
			= texture;
		segment->data.data.portal.padding_texture_bottom.texture
			= texture;
	}
	if (texture == NULL)
	{
		printf("Error: texture not found\n");
	}
}
