/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olaktion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 13:02:09 by olaktion          #+#    #+#             */
/*   Updated: 2019/04/11 13:18:06 by olaktion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	SDL_Color 	textColor = {255, 255, 255, 0};
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
	SDL_Rect        rect1, rect2, rect3;
    SDL_Rect        rect4, rect5, rect6; 
	SDL_Renderer    *renderer;
	SDL_Texture     *texture1, *texture2, *texture3;
    SDL_Texture     *num1, *num2, *num3;
	SDL_Window      *window;
	char            *font_path;
	int             quit;
	//
	const Uint8     *keystate;
	keystate = SDL_GetKeyboardState(NULL);
    int     i;
    i = 40;

    char   *test;
    test = ft_itoa(i); 
	//
	/*Font*/
	if (argc == 1) 
		font_path = "opensans.ttf";
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
	TTF_Font *font = TTF_OpenFont(font_path, 18);
	if (font == NULL)
	{
		ft_putstr("error: font not found\n");
		exit(EXIT_FAILURE);
	}
	get_text_and_rect(renderer, WINDOW_WIDTH - (WINDOW_WIDTH - 20), WINDOW_HEIGHT - 35,
        "Lives: ", font, &texture1, &rect1);

    get_text_and_rect(renderer, rect1.x + rect1.w + 5, WINDOW_HEIGHT - 35,
        test, font, &num1, &rect4);

	get_text_and_rect(renderer, WINDOW_WIDTH - (WINDOW_WIDTH / 2 + 50), 10 ,
        "Level: ", font, &texture2, &rect2);

    get_text_and_rect(renderer, rect2.x + rect2.w + 5, 10,
        test, font, &num2, &rect5);

	get_text_and_rect(renderer, WINDOW_WIDTH - 140, WINDOW_HEIGHT - 35,
        "Ammo: ", font, &texture3, &rect3);

    get_text_and_rect(renderer, rect3.x + rect3.w + 5, WINDOW_HEIGHT - 35,
        test, font, &num3, &rect6);
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
		SDL_RenderCopy(renderer, texture2, NULL, &rect2);
		SDL_RenderCopy(renderer, texture3, NULL, &rect3);
        SDL_RenderCopy(renderer, num1, NULL, &rect4);
        SDL_RenderCopy(renderer, num2, NULL, &rect5);
        SDL_RenderCopy(renderer, num3, NULL, &rect6);
		SDL_RenderPresent(renderer);
	}
	/* Deinit TTF.*/
	SDL_DestroyTexture(texture1);
	SDL_DestroyTexture(texture2);
	SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(num1);
    SDL_DestroyTexture(num2);
    SDL_DestroyTexture(num3);
	TTF_Quit();
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (EXIT_SUCCESS);
}
	/* gcc hud.c -o hud -I ./frameworks/SDL2.framework/Headers -I ./frameworks/SDL2_ttf.framework/Headers -rpath @loader_path/frameworks -F./frameworks/ -framework SDL2  -framework SDL2_ttf */
