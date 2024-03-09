/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_player_hold_logic.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:32:23 by olimarti          #+#    #+#             */
/*   Updated: 2024/03/04 22:53:46 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dynamic_array.h"
#include "game_loop.h"
#include "maths_utils.h"
#include "matrix.h"
#include "settings.h"
#include "structures.h"
#include "raycaster.h"
#include <AL/al.h>
#include <AL/alc.h>


void	take_item(t_entity *self, t_game_data *game_data, int item_id)
{
	t_entity_player_data	*data;
	t_entity				*item;

	data = self->data;
	item = sparse_array_get(game_data->state.entities, item_id);
	if (item == NULL)
		return;
	if (item->type == ENTITY_CANDLE)
	{
		data->holding_item = true;
		data->holding_item_id = item_id;
	}
}

static double	distance_3d(t_vector4d a, t_vector4d b)
{
	return (sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)));
}

int		is_item_aimed(t_entity *self, t_game_data *game_data, int item_id)
{
	t_entity_player_data	*data;
	t_entity				*item;
	t_vector4d				vec;
	int						dot;
	int						dist;

	data = self->data;
	item = sparse_array_get(game_data->state.entities, item_id);
	if (item == NULL)
		return (0);
	vec.vec = item->physics.pos.vec - self->physics.pos.vec;
	dot = dot_product_2d(&self->physics.dir, &vec);
	if (dot < 0.9)
		return (0);
	dist = distance_3d(self->physics.pos, item->physics.pos);
	return (dist < DEFAULT_PLAYER_TAKE_DISTANCE);
}

void	take_near_item(t_entity *self, t_game_data *game_data)
{
	t_entity_player_data	*data;
	t_entity				*item;
	int						i;

	data = self->data;
	i = 0;
	while (i < game_data->state.entities->size)
	{
		item = sparse_array_get(game_data->state.entities, i);
		if (item && item->type == ENTITY_CANDLE) //TODO: use more flexible system
		{
			if (is_item_aimed(self, game_data, i))
			{
				take_item(self, game_data, i);
				return ;
			}
		}
		i++;
	}
}

void update_holded_item(t_entity *self, t_game_data *game_data)
{
	t_entity_player_data *data;
	t_entity *item;

	data = self->data;
	if (data->holding_item == false)
		return;
	item = sparse_array_get(game_data->state.entities, data->holding_item_id);
	if (item == NULL)
	{
		data->holding_item = false;
		data->holding_item_id = -1;
		return;
	}

	item->physics.pos.x = (self->physics.pos.x + self->physics.dir.x * 0.7) * 0.5 + item->physics.pos.x * 0.5;
	item->physics.pos.y = (self->physics.pos.y + self->physics.dir.y * 0.7) * 0.5 + item->physics.pos.y * 0.5;
	item->physics.pos.z = (self->physics.pos.z + 0.3) * 0.5 + item->physics.pos.z * 0.5;
	item->physics.dir.x = -(self->physics.dir.x) * 1 + item->physics.dir.x * 0.0;
	item->physics.dir.y = -(self->physics.dir.y) * 1 + item->physics.dir.y * 0.0;
}

double	get_floor_height(t_entity *self, t_game_data *game_data)
{
	double					floor_height;
	t_bsp_tree_node_data	*sector;
	t_tree_node				*node;

	node = bsp_search_point_fast(game_data->map_data.bsp, &self->physics.pos);
	if (node == NULL)
		return (0);
	sector = node->data;
	floor_height = sector->sector_data.floor - self->physics.height / 2;
	return (floor_height);
}

void	drop_item(t_entity *self, t_game_data *game_data)
{
	t_entity_player_data	*data;
	t_entity				*item;
	double					floor_height;

	data = self->data;
	if (data->holding_item == false)
		return;
	item = sparse_array_get(game_data->state.entities, data->holding_item_id);
	if (item == NULL)
	{
		data->holding_item = false;
		data->holding_item_id = -1;
		return;
	}
	item->physics.pos.x = self->physics.pos.x + self->physics.dir.x * 0.7;
	item->physics.pos.y = self->physics.pos.y + self->physics.dir.y * 0.7;
	item->physics.pos.z = get_floor_height(item, game_data);
	item->physics.dir.x = -self->physics.dir.x;
	item->physics.dir.y = -self->physics.dir.y;
	item->physics.dir.z = 0;
	data->holding_item = false;
	data->holding_item_id = -1;
}

void	entity_player_update_holding(t_entity *self, t_game_data *game_data)
{
	t_entity_player_data	*data;

	data = self->data;
	if (game_data->inputs->action_states_once[a_interact] == 1)
	{
		printf("interact\n");
		if (data->holding_item == false)
		{
			printf("take_near_item\n");
			take_near_item(self, game_data);
		}
		else
		{
			drop_item(self, game_data);
		}
	}
	update_holded_item(self, game_data);
}
