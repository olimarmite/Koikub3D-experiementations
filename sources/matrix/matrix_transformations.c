/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transformations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 17:21:17 by motero            #+#    #+#             */
/*   Updated: 2023/10/14 16:41:39 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

//max a better cos and sin funciton later
t_matrix3x3	rotation_matrix(double theta)
{
	const double	cos_theta = cos(theta);
	const double	sin_theta = sin(theta);
	t_matrix3x3		rotation;

	rotation.row[0].vec = (t_v4d){cos_theta, -sin_theta, 0, 0};
	rotation.row[1].vec = (t_v4d){sin_theta, cos_theta, 0, 0};
	rotation.row[2].vec = (t_v4d){0, 0, 1, 0};
	return (rotation);
}

t_matrix3x3	scaling_matrix(t_point2d scale)
{
	t_matrix3x3	scaling;

	scaling.row[0].vec = (t_v4d){scale.x, 0, 0, 0};
	scaling.row[1].vec = (t_v4d){0, scale.y, 0, 0};
	scaling.row[2].vec = (t_v4d){0, 0, 1, 0};
	return (scaling);
}

t_matrix3x3	translation_matrix(t_point2d translation)
{
	t_matrix3x3	trans;

	trans.row[0].vec = (t_v4d){1, 0, translation.x, 0};
	trans.row[1].vec = (t_v4d){0, 1, translation.y, 0};
	trans.row[2].vec = (t_v4d){0, 0, 1, 0};
	return (trans);
}

int	push_matrix_op(t_list *matrix_op, t_matrix3x3 matrix)
{
	t_list	*new_matrix_op;

	new_matrix_op = ft_lstnew(&matrix);
	if (!new_matrix_op)
		return (1);
	ft_lstadd_front(matrix_op, new_matrix_op);
	return (0);
}