/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvoronyu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 19:46:48 by tvoronyu          #+#    #+#             */
/*   Updated: 2019/01/24 21:01:14 by tvoronyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_error(int error, t_wolf *wolf)
{
    if (error == 1)
        ft_putendl("Error !");
    else
        ft_putendl("Error map !");
    wolf = NULL;
    exit(0);
}

void	ft_init_all(t_wolf *wolf)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ft_error(3, wolf);
	if (!(wolf->sdl.win = SDL_CreateWindow("DOOM Kurva!",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, WIDTH,
					HEIGHT, SDL_WINDOW_RESIZABLE
					| SDL_WINDOW_SHOWN)))
		ft_error(3, wolf);
	if (!(wolf->sdl.ren = SDL_CreateRenderer(wolf->sdl.win, -1,
					SDL_RENDERER_ACCELERATED
					| SDL_RENDERER_PRESENTVSYNC)))
		ft_error(3, wolf);
	if (!(wolf->sdl.screen = SDL_CreateTexture(wolf->sdl.ren,
					SDL_PIXELFORMAT_ARGB8888,
					SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT)))
		ft_error(3, wolf);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		ft_error(3, wolf);
	if (TTF_Init() < 0)
		ft_error(3, wolf);
	ft_init_varible(wolf);
}

//vzamyati's validation

static void		add_elem(t_buff **lst, char *buff)
{
	t_buff *temp;

	if (!*lst)
	{
		*lst = (t_buff *)ft_memalloc(sizeof(t_buff));
		(*lst)->line = ft_strdup(buff);
	}
	else
	{	temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = (t_buff *)ft_memalloc(sizeof(t_buff));
		temp = temp->next;
		temp->line = ft_strdup(buff);
	}
}

static void		read_file_m(t_buff **lst, char *av, t_wolf *wolf)
{
	int fd;
	char *buff;

	if ((fd = open(av, O_RDONLY)) < 0 )
		ft_error(2, wolf);
	while (get_next_line(fd, &buff))
	{
		add_elem(lst, buff);
		ft_strdel(&buff);
	}
}

static size_t	ft_linelen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	return (i);
}

static size_t		ft_bufflen(t_buff *lst)
{
	t_buff	*tmp;
	size_t	len;

	len = 0;
	tmp = lst;
	if (lst)
		while (tmp)
		{
			len++;
			tmp = tmp->next;
		}
	return (len);
}

static void			open_file_textures(char *argv, t_wolf *wolf)
{
	int			i;
	int			len;
	t_buff		*line;

	i = 0;
	line = NULL;
	printf("here\n");
	read_file_m(&line, argv, wolf);
	wolf->MAP_WIDTH = (int)ft_linelen(line->line);
	wolf->MAP_HEIGHT = (int)ft_bufflen(line);
	printf("height - %d, width - %d\n", wolf->MAP_HEIGHT, wolf->MAP_WIDTH);
	// validate_map1(line, i, wolf);
	// make_map(av, i, w);
	// validate_map2(w);
}

int		main(int argc, char *argv[])
{
	t_wolf *wolf;

	if (argc == 2)
	{
		wolf = (t_wolf*)malloc(sizeof(t_wolf));
		// vzamyati's validation
		open_file_textures(argv[1], wolf);
		ft_bzero(wolf, sizeof(t_wolf));
		ft_init_all(wolf);
        createTables(wolf);
        writeTables(wolf);
        // creatMap(wolf);
		ft_game(wolf);
	}
	else
		ft_putendl("Дебіл! :) ");
//	system("leaks wolf3d");
	return (0);
}
