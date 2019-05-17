#include "doom.h"
#include <fcntl.h>


void	pixel_put(t_sdl *sdl, int x, int y, int color)
{
	// int		*pixel;

	if (x >= 0 && x < sdl->surface->w && y >= 0 && y < sdl->surface->h)
	{
		*((Uint32*)(sdl->surface->pixels) + x + y * sdl->surface->w) = color;
		// pixel = sdl->surface->pixels + y * sdl->surface->pitch + x * sdl->surface->format->BytesPerPixel;
		// *pixel = color;
	}
}

void				set_grid(t_sdl *sdl, int div)
{
	t_point p;

	p.y = 30;
	while (p.y < WIN_H - 15)
	{
		p.x = 30;
		while (p.x < WIN_W - 390)
		{
			pixel_put(sdl, p.x, p.y, BLACK);
			p.x += div;
		}
		p.y += div;
	}
}

static void		set_bg(t_sdl *sdl)
{
	t_point			p;
	const int		d = 5;
	const t_point	px = {900, 1260};
	const t_point	py = {25, 700};

	p.y = -1;
	while (++p.y < WIN_H)
	{
		p.x = -1;
		while (++p.x < WIN_W)
			pixel_put(sdl, p.x, p.y, BLUE);
	}
	p.y = py.x;
	while (++p.y < py.y)
	{
		p.x = px.x;
		while (++p.x < px.y)
		{
			if (p.x <= (px.x + d) || p.x >= (px.y - d) || p.y <= (py.x + d)
				|| p.y >= (py.y - d))
				pixel_put(sdl, p.x, p.y, BLACK);
			else
				pixel_put(sdl, p.x, p.y, YELLOW);
		}
	}
}

void				update(t_sdl *sdl, int div)
{
	SDL_Event		event;

	int				update;

	update = 1;
	// (void)div;
	while (update)
	{
		set_bg(sdl);
		// set_grid(sdl);
		set_grid(sdl, div);
		while (SDL_PollEvent(&event)) 
		{
			if ((event.type == SDL_QUIT) || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
				update = 0;
			// тут добавить if (event.type == SDL_MOUSEBUTTONDOWN) и реакцию на нажатие 
		}
		SDL_UpdateWindowSurface(sdl->win);
	}
}

void				terminate(t_sdl *sdl)
{
	SDL_DestroyWindow(sdl->win);
	SDL_Quit();
}

t_sdl				*init_sdl(void)
{
	t_sdl			*sdl;

	sdl = (t_sdl *)malloc(sizeof(t_sdl));
	SDL_Init(SDL_INIT_EVERYTHING);
	sdl->win = SDL_CreateWindow("Doom_Nukem 2.0 Map Editor",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIN_W,
		WIN_H,
		SDL_WINDOW_ALLOW_HIGHDPI); //window should be created in high-DPI mode if supported
	sdl->surface = SDL_GetWindowSurface(sdl->win);
	return (sdl);
}

void	ft_error(char *str, int error, void (*f)(const char *))
{
	f(str);
	exit(error);
}

void	start_editor(char *map_file)
{
	t_sdl			*sdl;
	int				div;
	int fd;

	if ((fd = open(map_file, O_RDONLY)) == -1)
		ft_error("error", 2, perror);
	div = WIN_W / 50; 
	sdl = init_sdl();
	update(sdl, div);
	terminate(sdl);
}

int		create_map_file(char *name)
{
	int fd;

	if ((fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1)
		ft_error("error", 1, perror);
	ft_putstr_fd("we've created!\n", fd);
	return (1);
}

int					main(int ac, char **av)
{
	if (ac == 3 && av[1][0] == '-' && av[1][1] == 'n' && av[1][2] == '\0')
	{
		create_map_file(av[2]);
		start_editor(av[2]);
	}
	else if (ac == 3 && av[1][0] == '-' && av[1][1] == 'e' && av[1][2] == '\0')
		start_editor(av[2]);
	else
	{
		ft_putstr_fd("usage: ./map_editor [-n || -e] [map_file] \n", 1);
	}
	return (0);
}