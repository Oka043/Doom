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

			void		validate_map1(t_buff *line, int i, t_wolf *wolf)
			{
				int			len;
				t_buff		*tmp;

				len = 0;
				tmp = line;
				while (tmp)
				{
					while (tmp->line[i] && tmp->line[i] != '\0')
					{
						if ((tmp->line[i] < 48 || tmp->line[i] > 57) && tmp->line[i] != ' ' && tmp->line[i] != '\n')
							ft_error(1, wolf);
						len++;
						if (tmp->line[i] == '\n')
						{
							if (len - 1 != wolf->MAP_WIDTH)
								ft_error(1, wolf);
							len = 0;
						}
						i++;
					}
					tmp = tmp->next;
					i = 0;
				}
				//почистить здесь список от ликов
				tmp = line;
				while (tmp->line[i] != '\0' && tmp->line[i] != '\n')
				{
					if (tmp->line[i] == ' ')
						wolf->MAP_WIDTH--;
					i++;
				}
				// ft_strdel(&buf);
			}

			int		ft_isspace(int c)
			{
				if ((c == ' ') || (c >= '\t' && c <= '\r'))
					return (1);
				return (0);
			}


			static long		ft_atol_base2(const char *str, int base)
			{
				long		res;
				char		*dig;
				char		*tmp;
				int			i;

				dig = "0123456789abcdefghijklmnopqrstuvwxyz";
				res = 0;
				i = 0;
				while (str[i] && (tmp = (char *)ft_memchr(dig, ft_tolower(str[i]), base)))
				{
					res = (res * base) + (tmp - dig);
					i++;
				}
				return (res);
			}

			long			ft_atol_base(const char *str, int base)
			{
				long		res;
				char		sign;
				int			i;

				sign = 1;
				i = 0;
				res = 0;
				if (!str || !*str || base < 2 || base > 36)
					return (res);
				while (ft_isspace(str[i]))
					i++;
				sign = (base == 10 && str[i] == '-') ? -1 : 1;
				if (base == 10 && (*str == '-' || *str == '+'))
					i++;
				if (base == 16 && str[i] == '0' && ft_tolower(str[i + 1]) == 'x')
					res = ft_atol_base2(str + 2, base);
				else
					res = ft_atol_base2(str, base);
				return (res * sign);
			}

			void		make_map(char *argv, int i, t_wolf *wolf)
			{
				char		*line;
				int			j;
				int			fd;
				int			k;

				fd = open(argv, O_RDONLY);
				if (!(wolf->array = (int **)ft_memalloc(sizeof(int *) * wolf->MAP_HEIGHT)))
					ft_error(1, wolf);
				while (get_next_line(fd, &line) > 0)
				{
					j = -1;
					k = 0;
					if (!(wolf->array[i] = ft_memalloc(sizeof(int) * wolf->MAP_WIDTH)))
						ft_error(1, wolf);
					while (++j < wolf->MAP_WIDTH)
					{
						if (line[k] == ' ')
							k++;
						wolf->array[i][j] = ft_atol_base(&line[k], 10);
						// check_value(w, i, j); чек на макс.инт
						k++;
					}
					i++;
					ft_strdel(&line);
				}
			}

			int			get_position(t_wolf *wolf, int x, int y)
			{
				if (x < 0 || y < 0 || x > wolf->MAP_WIDTH - 1 || y > wolf->MAP_HEIGHT - 1)
					return (0);
				return (wolf->array[y][x]);
			}

			int			check_position(t_wolf *wolf)
			{
				int		i;
				int		j;
				int		v;

				i = -1;
				j = -1;
				v = 0;
				while (++i < wolf->MAP_HEIGHT)
				{
					j = -1;
					while (++j < wolf->MAP_WIDTH)
						if ((v = get_position(wolf, j, i)) == 0)
							break ;
					if (!v)
						return (1);
				}
				return (0);
			}

			void		validate_map2(t_wolf *wolf)
			{
				int			i;

				i = -1;
				while (++i < wolf->MAP_WIDTH)
					if (wolf->array[0][i] == 0 || wolf->array[wolf->MAP_HEIGHT - 1][i] == 0)
						ft_error(1, wolf);
				i = -1;
				while (++i < wolf->MAP_HEIGHT)
					if (wolf->array[i][0] == 0 || wolf->array[i][wolf->MAP_WIDTH - 1] == 0)
						ft_error(1, wolf);
				if (!check_position(wolf))
					ft_error(1, wolf);
			}

			static void			open_file_textures(char *argv, t_wolf *wolf)
			{
				int			i;
				int			len;
				t_buff		*line;

				i = 0;
				line = NULL;
				read_file_m(&line, argv, wolf);
				wolf->MAP_WIDTH = (int)ft_linelen(line->line);
				wolf->MAP_HEIGHT = (int)ft_bufflen(line);
				validate_map1(line, i, wolf);
				make_map(argv, i, wolf);
				validate_map2(wolf);
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
	system("leaks doom");
	return (0);
}
