/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaktion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 13:09:52 by olaktion          #+#    #+#             */
/*   Updated: 2019/06/12 13:09:54 by olaktion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parts.h"

void			parse_sec(t_main *main, t_list *list)
{
	t_list	*elem;
	size_t	len;
	size_t	i;

	i = 0;
	elem = find_elem(list, "sector");
	len = content_len(elem, "sector");
	main->sec = allocate_sec(len);
	while (elem && i < len)
	{
		main->sec[i] = processing(main, i, (char *)elem->content);
		elem = elem->next;
		i++;
	}
	main->sum_sect = len; // 25 cчитает нормально
}

t_sec			*allocate_sec(size_t size)
{
	t_sec *sec;

	if (!(sec = (t_sec *)ft_memalloc(sizeof(t_sec) * size)))
		print_error("Error can't allocate memory region");
	return (sec);
}

t_sec	processing(t_main *main, size_t index, char *sec_data)
{
	t_sec		sec;
	char		**dat_arr;

	ft_bzero(&sec, sizeof(t_sec));
	if (sec_data)
	{
		dat_arr = ft_strsplit(ft_strchr(sec_data, '\t'), '\t');
		if (two_dim_len(dat_arr) == 3)
		{
			sec.color = 0;
			sec.sec_index = index;
			printf("%d\n", index);
			sec.height = parse_height_params(dat_arr[M1(1)]); // везде пишет 0 ?
			printf("%d\n", sec.height.floor);
			sec.vertex = pars_ver_par(dat_arr[M1(2)], main, &sec.ver_arr_len);
			sec.sec_dots = pars_extra_par(dat_arr[M1(3)], sec.ver_arr_len);
			two_dim_del(&dat_arr);
		}
		else
			print_error("Error, wrong sector param number1");
	}
	return (sec);
}

t_bool			integer_overload_error(char **data)
{
	while (*data)
	{
		if (!ft_isint(*data, false) && !ft_strchr(*data, '.'))
			print_error("Error, int overload");
		data++;
	}
	return (false);
}

double			**allocate_vertex_array(size_t size)
{
	double **array;

	if (!(array = (double **)ft_memalloc(sizeof(double *) * size + 1)))
		print_error("Error, can't allocate memory region");
	return (array);
}
