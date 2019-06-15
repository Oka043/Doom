/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spetrenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 17:51:56 by spetrenk          #+#    #+#             */
/*   Updated: 2018/11/05 17:52:00 by spetrenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_NUKEM_FT_MATH_H
# define DOOM_NUKEM_FT_MATH_H

/*
** MIN:		Choose smaller of two scalars.
** MAX:		Choose greater of two scalars.
** CLAMP:	Clamp value into set range.
** VXS:		Vector cross product
** V_SUB:	Vector sub
*/

# define MIN(a,b)             (((a) < (b)) ? (a) : (b))
# define MAX(a,b)             (((a) > (b)) ? (a) : (b))
# define CLAMP(a, mi,ma)     MIN(MAX(a,mi),ma)
# define VXS(x0,y0, x1,y1)    ((x0)*(y1) - (x1)*(y0))
# define V_SUB(v1, v2) ((t_vector) {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z})


// Intersect: Calculate the point of intersection between two lines.
#define Intersect(x1,y1, x2,y2, x3,y3, x4,y4) ((t_vector) { \
    VXS(VXS(x1,y1, x2,y2), (x1)-(x2), VXS(x3,y3, x4,y4), (x3)-(x4)) / VXS((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)), \
    VXS(VXS(x1,y1, x2,y2), (y1)-(y2), VXS(x3,y3, x4,y4), (y3)-(y4)) / VXS((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) })

/*
** v2_rotate:	Rotate 2 val vector around using already calculated cos and sin
*/

//
//typedef struct	s_vector
//{
//	float x;
//	float y;
//	float z;
//}				t_vector;

t_vector		v2_rotate(t_vector v, float acos, float asin);

#endif
