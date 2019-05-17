#include "hud.h"

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

void	print_menu(t_hud *hud)
{
	int 	i;

	i = 0;
	hud->keystate = SDL_GetKeyboardState(NULL);
	hud->font_path = "./fonts/Exo2.0-ExtraLight.otf";

	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &hud->window, &hud->renderer);
	TTF_Font *font = TTF_OpenFont(hud->font_path, 30);
	if (font == NULL)
	{
		ft_putstr("error: font not found\n");
		exit(EXIT_FAILURE);
	}

	get_text_and_rect(hud->renderer, WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100 ,
        "NEW GAME", font, &hud->texture1, &hud->rect1);
	get_text_and_rect(hud->renderer, WINDOW_WIDTH / 2 - 100, hud->rect1.y + hud->rect1.h + 20,
        "INSTRUCTION", font, &hud->texture2, &hud->rect2);
	get_text_and_rect(hud->renderer, WINDOW_WIDTH / 2 - 100, hud->rect2.y + hud->rect2.h + 20,
        "EXIT ", font, &hud->texture3, &hud->rect3);

	hud->quit = 0;
	while (!hud->quit)
	{
	   	while (SDL_PollEvent(&hud->event) == 1) 
		{
			if (hud->event.type == SDL_QUIT ||
					hud->keystate[SDL_SCANCODE_ESCAPE]) 
				hud->quit = 1; 
		}

		if (i == 0)
			get_text_and_rect(hud->renderer, WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100,
        		"*	"  , font, &hud->texture4, &hud->rect4);
		else if (i == 1)
			get_text_and_rect(hud->renderer, WINDOW_WIDTH / 2 - 150, hud->rect1.y + hud->rect1.h + 20,
        		"*	"  , font, &hud->texture4, &hud->rect4);
		else if (i == 3)
			get_text_and_rect(hud->renderer, WINDOW_WIDTH / 2 - 150, hud->rect2.y + hud->rect2.h + 20,
        		"*	"  , font, &hud->texture4, &hud->rect4);

		SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 0);
		SDL_RenderClear(hud->renderer);
		/*Use textures.*/
		SDL_RenderCopy(hud->renderer, hud->texture1, NULL, &hud->rect1);
		SDL_RenderCopy(hud->renderer, hud->texture2, NULL, &hud->rect2);
		SDL_RenderCopy(hud->renderer, hud->texture3, NULL, &hud->rect3);
		SDL_RenderCopy(hud->renderer, hud->texture4, NULL, &hud->rect4);

		SDL_RenderPresent(hud->renderer);
	}

	SDL_DestroyTexture(hud->texture1);
	SDL_DestroyTexture(hud->texture2);
	SDL_DestroyTexture(hud->texture3);
	SDL_DestroyTexture(hud->texture4);

	SDL_DestroyRenderer(hud->renderer);

	SDL_DestroyWindow(hud->window);
}


int main(void) 
{
	t_hud	*hud;
	hud = malloc(sizeof(t_hud));


	/*Inint TTF.*/
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	TTF_Init();

	print_menu(hud);
	
	/* Deinit TTF.*/
	TTF_Quit();
	SDL_Quit();
	return (EXIT_SUCCESS);
}

/* gcc menu2.c -o menu2 -I ./frameworks/SDL2.framework/Headers -I ./frameworks/SDL2_ttf.framework/Headers -rpath @loader_path/frameworks -F./frameworks/ -framework SDL2  -framework SDL2_ttf */