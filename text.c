#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "./libft/ft_putstr.c"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT (WINDOW_WIDTH)

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

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

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Rect rect1, rect2, rect3;
    SDL_Renderer *renderer;
    SDL_Texture *texture1, *texture2, *texture3;
    SDL_Window *window;
    char *font_path;
    int quit;

    //
    const Uint8     *keystate;
    keystate = SDL_GetKeyboardState(NULL);    
    //

    /*Font*/
    if (argc == 1) 
        font_path = "opensanslightitalic.ttf";
    else if (argc == 2)
        font_path = argv[1];
    else 
    {
        ft_putstr("error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 12);
    if (font == NULL)
     {
        ft_putstr("error: font not found\n");
        exit(EXIT_FAILURE);
    }

    get_text_and_rect(renderer, WINDOW_WIDTH - (WINDOW_WIDTH - 20),  WINDOW_HEIGHT - 30, "Lives:", font, &texture1, &rect1);
    get_text_and_rect(renderer, WINDOW_WIDTH - (WINDOW_WIDTH / 2), 10 , "Level:", font, &texture2, &rect2);
    get_text_and_rect(renderer, WINDOW_WIDTH - 100, WINDOW_HEIGHT - 30, "Ammo:", font, &texture3, &rect3);


    // get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2, &rect2);

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) 
        {
            if (event.type == SDL_QUIT ||
                         keystate[SDL_SCANCODE_ESCAPE]) 
                quit = 1;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        /* Use textures. */
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        SDL_RenderCopy(renderer, texture3, NULL, &rect3);
        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}