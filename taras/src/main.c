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

int		main(int argc, char *argv[])
{
	t_wolf *wolf;

	argv = NULL;
	if (argc == 2)
	{
		wolf = (t_wolf*)malloc(sizeof(t_wolf));
		ft_bzero(wolf, sizeof(t_wolf));
		ft_init_all(wolf);
        createTables(wolf);
        writeTables(wolf);
        creatMap(wolf);
		ft_game(wolf);
	}
	else
		ft_putendl("Дебіл! :) ");
//	system("leaks wolf3d");
	return (0);
}
