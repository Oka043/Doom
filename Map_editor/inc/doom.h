#ifndef DOOM_H
# define DOOM_H

# define WIN_W		1280
# define WIN_H		720

# include <SDL.h>
# include <math.h>
# include <fcntl.h>
# include "../libft/libft.h"

# define BLACK		(Uint32)0x000000
# define WHITE		(Uint32)0xFFFFFF
# define BLUE		(Uint32)0x6FA8DC
# define GREY		(Uint32)0x555555
# define RED		(Uint32)0x990000
# define GREEN		(Uint32)0x93C47D
# define LIGHT_GREY	(Uint32)0xBBBBBB
# define YELLOW		(Uint32)0xFFD966
# define ORANGE		(Uint32)0xFFA500
# define PURPLE		(Uint32)0x642EFE
# define PINK		(Uint32)0xC71585

typedef struct 	s_point
{
	int			x;
	int			y;
}				t_point;

typedef struct		s_sdl
{
	SDL_Window*		win;
	SDL_Surface* 	surface;
	SDL_Renderer*	render;
	SDL_Texture*	img_texture;
	SDL_Rect		texture_destination;
}					t_sdl;

void				update(t_sdl *sdl, int div);

// void				set_grid(t_sdl *sdl, int div);

void				pixel_put(t_sdl *sdl, int x, int y, int color);

#endif
