/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sector2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaktion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 13:10:05 by olaktion          #+#    #+#             */
/*   Updated: 2019/06/12 13:11:26 by olaktion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parts.h"

t_height		parse_height_params(char *data)
{
	t_height	height;
	char		**tmp;

	ft_bzero(&height, sizeof(t_height));
	if (data)
	{
		tmp = ft_strsplit(data, ' ');
		if (two_dim_len(tmp) == 2 && !integer_overload_error(tmp))
		{
			height.floor = ft_atoi(tmp[M1(1)]);
			height.cell = ft_atoi(tmp[M1(2)]);
		}
		else
			print_error("Error, wrong sector height param");
		two_dim_del(&tmp);
	}
	return (height);
}

double			**parse_vertex_pair(t_main *main, char **data, size_t size)
{
	double	**vertex_array;
	int32_t	val;
	size_t	i;

	i = 0;
	vertex_array = allocate_vertex_array(size);
	while (i < size)
	{
		val = ft_atoi(data[i]);
		if (ABS(val) < main->sum_vert_pair)
			vertex_array[i] = search_vertex_pair(main, val);
		else
			print_error("Error, no vertex pair");
		i++;
	}
	return (vertex_array);
}

double			*search_vertex_pair(t_main *main, int32_t pair)
{
	double	*vertex_pair;
	size_t	i;
	size_t	j;

	i = 0;
	vertex_pair = allocate_vertex_pair();
	printf("%s\n", vertex_pair );
	while (i < main->sum_vert)
	{
		j = 0;
		while (j < main->vertex[i].x_arr_size)
		{
			if (pair == 0)
			{
				vertex_pair[0] = main->vertex[i].y;
				vertex_pair[1] = main->vertex[i].x_arr[j];
				return (vertex_pair);
			}
			pair--;
			j++;
		}
		i++;
	}
	return (NULL);
}

double			*allocate_vertex_pair(void)
{
	double	*vertex;

	if (!(vertex = (double *)ft_memalloc(sizeof(double) * 2)))
		print_error("Error, can't allocate memory region");
	return (vertex);
}

int32_t			*pars_extra_par(char *data, size_t ver_arr_len)
{
	int32_t		*trans;
	char		**tmp;

	trans = NULL;
	if (data)
	{
		tmp = ft_strsplit(data, ' ');
		if (two_dim_len(tmp) == ver_arr_len && !integer_overload_error(tmp))
			trans = parse_extra_pair(tmp, ver_arr_len);
		else
			print_error("Error, sector walidation, bad params");
		two_dim_del(&tmp);
	}
	return (trans);
}
