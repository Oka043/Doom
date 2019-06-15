/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaktion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 13:08:39 by olaktion          #+#    #+#             */
/*   Updated: 2019/06/12 13:08:41 by olaktion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parts.h"

void		parse_player(t_main *main, t_list *list)
{
	t_list *elem;

	elem = find_elem(list, "player");
	main->player = parse_player_parameters((char *)elem->content);
	printf("%d", main->player.sec);
}

t_tplayer	parse_player_parameters(char *data)
{
	char		**tmp;
	t_tplayer	player;

	ft_bzero(&player, sizeof(t_tplayer));
	if (data)
	{
		tmp = ft_strsplit(ft_strchr(data, '\t'), '\t');
		if (two_dim_len(tmp) == 2)
		{
			player.pos = define_pos(tmp[M1(1)]);
			if (!integer_overload_error(&tmp[M1(2)]))
				player.sec = ft_atoi(tmp[M1(2)]);
		}
		else
			print_error("Error, bad vector position");
		two_dim_del(&tmp);
	}
	return (player);
}

t_vertex	define_pos(char *data)
{
	t_vertex	pos;
	char		**tmp;

	ft_bzero(&pos, sizeof(t_vertex));
	if (data)
	{
		tmp = ft_strsplit(data, ' ');
		if (two_dim_len(tmp) == 3 && !integer_overload_error(tmp))
		{
			pos.x = ft_atoi(tmp[M1(1)]);
			pos.y = ft_atoi(tmp[M1(2)]);
			pos.z = ft_atoi(tmp[M1(3)]);
		}
		else
			print_error("Error, bad coords for player");
		two_dim_del(&tmp);
	}
	return (pos);
}

int32_t			*parse_extra_pair(char **data, size_t size)
{
	int32_t		*extra;
	size_t i;

	i = 0;
	extra = allocate_extra_arr(size);
	while (i < size)
	{
		extra[i] = ft_atoi(data[i]);
		i++;
	}
	return (extra);
}

int32_t			*allocate_extra_arr(size_t size)
{
	int32_t	*extra;

	if (!(extra = (int32_t *)ft_memalloc(sizeof(int32_t) * size)))
		print_error("Can't allocate memory");
	return (extra);
}
