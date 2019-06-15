/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_with_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaktion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 13:10:37 by olaktion          #+#    #+#             */
/*   Updated: 2019/06/12 13:10:38 by olaktion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parts.h"
#include "../include/get_next_line.h"

void	read_file(t_main *main)
{
	char	*line;
	int32_t	fd;
	t_list	*list;

	list = NULL;
	if (!(fd = open("map.txt", O_RDONLY)))
							// потом убрать, это для проверки по существующей, правильной карте
		print_error("Error, bad file");
	while (get_next_line(fd, &line))
	{
		tmp_list(&list, (void *)line, ft_strlen(line));
		ft_strdel(&line);
	}
	parse_vertex(main, list); // все ок
	parse_sec(main, list); // все ок 
	parse_player(main, list);
	free_tmp_list(&list);
}

void	print_error(const char *msg)
{
	ft_putendl(msg);
	exit(0);
}

void	tmp_list(t_list **lst, const void *content,
		const size_t content_size)
{
	t_list	*tmp;

	if (!*lst)
		*lst = ft_lstnew(content, content_size);
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = ft_lstnew(content, content_size);
	}
}

void	free_tmp_list(t_list **head)
{
	t_list *tmp;

	while (*head)
	{
		tmp = *head;
		ft_memdel(&tmp->content);
		ft_memdel((void *)&tmp);
		(*head) = (*head)->next;
	}
	ft_memdel((void **)head);
}

void	init_all(t_main *main)
{
	read_file(main);
}