
#include "player_stable.h"
#include "../include/get_next_line.h"

void read_file(t_system *system)
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
	parse_vertex(system, list);
	parse_sec(system, list); // уже тут херится
//	transform_ver_arr(system);
	parse_player(system, list);

	system->sectors->ver_arr_len = 0;
	system->sum_vert = 0;
	system->sum_vert_pair = 0;
//	transform_ver_arr(system);
	free_tmp_list(&list);
}
//
//void transform_ver_arr(t_system *system)
//{
//	size_t   i;
//	size_t   j;
////	t_sector *sec;
//
//	i = 0;
//	j = 0;
////	system->sectors->vertex  = ft_memalloc()
//	system->sectors->vertex[j].x = system->sectors->vertex[i].m;
//	system->sectors->vertex[j].y = system->sectors->vertex[i].n;
//	i = 0;
//	j++;
//	while (system->sectors->vertex[i].m)
//	{
//		system->sectors->vertex[j].x = system->sectors->vertex[i].m;
//		i++;
//		j++;
//	}
//	i = 0;
//	j = 1;
//	while (system->sectors->vertex[i].n)
//	{
//		system->sectors->vertex[j].y = system->sectors->vertex[i].n;
//		i++;
//		j++;
//	}
//}

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
