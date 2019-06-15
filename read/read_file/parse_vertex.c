/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_vertex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaktion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 13:10:16 by olaktion          #+#    #+#             */
/*   Updated: 2019/06/12 13:10:17 by olaktion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parts.h"

void		parse_vertex(t_main *main, t_list *list)
{
	size_t	vertex_num;
	size_t	count;
	size_t	size_arr;
	t_list	*elem;

	count = 0;
	size_arr = 0;
	elem = find_elem(list, "vertex");
	vertex_num = content_len(elem, "vertex");
	main->vertex = allocate_vertex(vertex_num);

	while (elem && count < vertex_num)
	{
		if (validate_vertex((char *)elem->content))
		{
			main->vertex[count].y = parse_ver_y((char *)elem->content);
			main->vertex[count].x_arr = parse_ver_x((char *)elem->content, &size_arr);
			main->vertex[count].x_arr_size = size_arr;
			main->sum_vert_pair += size_arr;
			elem = elem->next;
			count++;
		}
		else
			print_error("Error, bad vertex validation1");
	}
	main->sum_vert = vertex_num;
}

t_list		*find_elem(t_list *list, char *elem)
{
	while (!ft_strstr((char *)list->content, elem))
		list = list->next;
	return (list);
}

size_t		content_len(t_list *list, char *content)
{
	size_t len;

	len = 0;
	while (list && ft_strstr((char *)list->content, content))
	{
		len++;
		list = list->next;
	}
	return (len);
}

t_vertex	*allocate_vertex(size_t size)
{
	t_vertex *vert_arr;

	if (!(vert_arr = (t_vertex *)ft_memalloc(sizeof(t_vertex) * size)))
		print_error("Error, bad memory region");
	return (vert_arr);
}

double		**pars_ver_par(char *data, t_main *main, size_t *len)
{
	double	**vertex_array;
	char	**tmp;

	vertex_array = NULL;
	if (data)
	{
		tmp = ft_strsplit(data, ' ');
		if (two_dim_len(tmp) >= 3 && !integer_overload_error(tmp))
		{
			*len = two_dim_len(tmp);
			vertex_array = parse_vertex_pair(main, tmp, *len);
		}
		else
			print_error("Error, sector walidation, bad vertex param");
		two_dim_del(&tmp);
	}
	return (vertex_array);
}
