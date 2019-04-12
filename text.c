#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "./libft/ft_putstr.c"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH)

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
		TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{
	int			text_width;
	int			text_height;
	SDL_Surface *surface;
	SDL_Color 	textColor = {224, 25, 25, 0};
    //
	surface = TTF_RenderText_Solid(font, text, textColor);
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);
	rect->x = x;
	rect->y = y;
	rect->w = text_width;
	rect->h = text_height;
}

int main(int argc, char **argv) 
{
	SDL_Event       event;
	SDL_Rect        rect1;
	SDL_Renderer    *renderer;
	SDL_Texture     *texture1; 
	SDL_Window      *window;
	char            *font_path;
	int             quit;
	//
	const Uint8     *keystate;
	keystate = SDL_GetKeyboardState(NULL);
	//

	/*Font*/
	if (argc == 1) 
		font_path = "beer_money.ttf";
	else if (argc == 2)
		font_path = argv[1];
	else
	{
		ft_putstr("error: too many arguments\n");
		exit(EXIT_FAILURE);
	}

	/*Inint TTF.*/
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
	TTF_Init();
	TTF_Font *font = TTF_OpenFont(font_path, 80);
	if (font == NULL)
	{
		ft_putstr("error: font not found\n");
		exit(EXIT_FAILURE);
	}
	
	get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 - 60 ,
        "YOU DIED ", font, &texture1, &rect1);

	/* get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2, &rect2); */
	quit = 0;
	while (!quit)
	{
	   	while (SDL_PollEvent(&event) == 1) 
		{
			if (event.type == SDL_QUIT ||
					keystate[SDL_SCANCODE_ESCAPE]) 
				quit = 1;
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		/*Use textures.*/
		SDL_RenderCopy(renderer, texture1, NULL, &rect1);

		SDL_RenderPresent(renderer);
	}
	/* Deinit TTF.*/
	SDL_DestroyTexture(texture1);

	TTF_Quit();
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (EXIT_SUCCESS);
}

/* gcc text.c -o text -I ./frameworks/SDL2.framework/Headers -I ./frameworks/SDL2_ttf.framework/Headers -rpath @loader_path/frameworks -F./frameworks/ -framework SDL2  -framework SDL2_ttf */