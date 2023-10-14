/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_visualizer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olimarti <olimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 16:55:23 by olimarti          #+#    #+#             */
/*   Updated: 2023/10/14 18:00:21 by olimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_VISUALIZER_H
# define BSP_VISUALIZER_H

# include "structures.h"
# include "draw_utils.h"

/*############################################################################*/
/*                                MAP DISPLAY                                 */
/*############################################################################*/
int		draw_map_segments(t_cub *data, t_list *segments_lst);
int		map_visualizer_render(t_cub *data);

#endif
