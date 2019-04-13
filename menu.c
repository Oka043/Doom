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
	SDL_Color 	textColor = {120, 144, 176, 0};
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

void	print_menu(void)
{
	SDL_Window      *window;
	SDL_Event       event;
	SDL_Rect        rect1, rect2, rect3, rect4;
	SDL_Renderer    *renderer;
	SDL_Texture     *texture1, *texture2, *texture3, *texture4; 
	int             quit;
	char            *font_path;
	const Uint8     *keystate;
	int 			i; // for choose option in menu

	keystate = SDL_GetKeyboardState(NULL);
	font_path = "./fonts/Exo2.0-ExtraLight.otf";

	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
	TTF_Font *font = TTF_OpenFont(font_path, 30);
	if (font == NULL)
	{
		ft_putstr("error: font not found\n");
		exit(EXIT_FAILURE);
	}

	get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100 ,
        "NEW GAME", font, &texture1, &rect1);
	get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 100, rect1.y + rect1.h + 20,
        "INSTRUCTION", font, &texture2, &rect2);
	get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 100, rect2.y + rect2.h + 20,
        "EXIT ", font, &texture3, &rect3);

	i = 1;
	if (i == 1)
		get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100,
        "*	"  , font, &texture4, &rect4);
	else if (i == 2)
		get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 150, rect1.y + rect1.h + 20,
        "*	"  , font, &texture4, &rect4);
	else if (i == 3)
		get_text_and_rect(renderer, WINDOW_WIDTH / 2 - 150, rect2.y + rect2.h + 20,
        "*	"  , font, &texture4, &rect4);

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
		SDL_RenderCopy(renderer, texture2, NULL, &rect2);
		SDL_RenderCopy(renderer, texture3, NULL, &rect3);
		SDL_RenderCopy(renderer, texture4, NULL, &rect4);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture1);
	SDL_DestroyTexture(texture2);
	SDL_DestroyTexture(texture3);
	SDL_DestroyTexture(texture4);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

int main(void) 
{
	/*Font*/
	// if (argc == 1) 
	// 	font_path = "./fonts/Exo2.0-ExtraLight.otf";
	// else if (argc == 2)
	// 	font_path = argv[1];
	// else
	// {
	// 	ft_putstr("error: too many arguments\n");
	// 	exit(EXIT_FAILURE);
	// }

	/*Inint TTF.*/
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	// SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
	TTF_Init();
	// TTF_Font *font = TTF_OpenFont(font_path, 30);
	// if (font == NULL)
	// {
	// 	ft_putstr("error: font not found\n");
	// 	exit(EXIT_FAILURE);
	// }
	print_menu();

	
	/* get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2, &rect2); */
	
	/* Deinit TTF.*/
	TTF_Quit();
	SDL_Quit();
	return (EXIT_SUCCESS);
}

/* gcc menu.c -o menu -I ./frameworks/SDL2.framework/Headers -I ./frameworks/SDL2_ttf.framework/Headers -rpath @loader_path/frameworks -F./frameworks/ -framework SDL2  -framework SDL2_ttf */