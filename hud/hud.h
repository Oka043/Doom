#ifndef HUD_H
# define HUD_H

#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "./libft/ft_putstr.c"
#include "./libft/ft_itoa.c"
#include "./libft/ft_minus.c"
#include "./libft/ft_strdup.c"
#include "./libft/ft_strlen.c"



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH)

# define BLUE		(Uint32)0x0000FF


typedef struct 	s_hud
{
	// int				x;
	// int 			y;
	// char			text;
	// TTF_Font		*font;
	SDL_Texture		**texture;
	SDL_Rect 		*rect;
	SDL_Surface 	*surface;
	// SDL_Renderer 	*renderer;
	TTF_Font 		*font;
	SDL_Color   	textColor;
	SDL_Event		event;
	
	SDL_Rect		rect1;
	SDL_Rect		rect2;
	SDL_Rect		rect3;
	SDL_Rect		rect4;
	SDL_Rect		rect5;
	SDL_Rect		rect6;

	SDL_Renderer    *renderer;
    SDL_Texture     *texture1;
    SDL_Texture     *texture2;
    SDL_Texture     *texture3;
    SDL_Texture     *texture4;
    SDL_Texture     *texture5;
	SDL_Texture     *texture6;

    SDL_Window      *window;
    char            *font_path;
    int             quit;
    const Uint8     *keystate;

}				t_hud;

int 	event(t_hud *hud);
void	print_menu(t_hud *hud);
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
		TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);

#endif