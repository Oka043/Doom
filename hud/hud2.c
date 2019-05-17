#include "hud.h"

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{
    int         text_width;
    int         text_height;
    SDL_Surface *surface;
    SDL_Color   textColor = {255, 255, 255, 0};
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

void    print_hud(t_hud *hud, int a, int b, int c)
{
    char    *lives;
    char    *ammo;
    char    *level;

    lives = ft_itoa(a);
    level = ft_itoa(b);
    ammo = ft_itoa(c);
    hud->keystate = SDL_GetKeyboardState(NULL);

    hud->font_path = "./fonts/opensans.ttf";

    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &hud->window, &hud->renderer);
    TTF_Font *font = TTF_OpenFont(hud->font_path, 18);
    if (font == NULL)
    {
        ft_putstr("error: font not found\n");
        exit(EXIT_FAILURE);
    }

    get_text_and_rect(hud->renderer, WINDOW_WIDTH - (WINDOW_WIDTH - 20), WINDOW_HEIGHT - 35,
        "Lives: ", font, &hud->texture1, &hud->rect1);

    get_text_and_rect(hud->renderer, hud->rect1.x + hud->rect1.w + 5, WINDOW_HEIGHT - 35,
        lives, font, &hud->texture4, &hud->rect4);

    get_text_and_rect(hud->renderer, WINDOW_WIDTH - (WINDOW_WIDTH / 2 + 50), 10 ,
        "Level: ", font, &hud->texture2, &hud->rect2);

    get_text_and_rect(hud->renderer, hud->rect2.x + hud->rect2.w + 5, 10,
        level, font, &hud->texture5, &hud->rect5);

    get_text_and_rect(hud->renderer, WINDOW_WIDTH - 140, WINDOW_HEIGHT - 35,
        "Ammo: ", font, &hud->texture3, &hud->rect3);

    get_text_and_rect(hud->renderer, hud->rect3.x + hud->rect3.w + 5, WINDOW_HEIGHT - 35,
        ammo, font, &hud->texture6, &hud->rect6);
    hud->quit = 0;
    while (!hud->quit)
    {
        while (SDL_PollEvent(&hud->event) == 1) 
        {
            if (hud->event.type == SDL_QUIT ||
                    hud->keystate[SDL_SCANCODE_ESCAPE]) 
                hud->quit = 1;
        }
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 0);
        SDL_RenderClear(hud->renderer);
        /*Use textures.*/
        SDL_RenderCopy(hud->renderer, hud->texture1, NULL, &hud->rect1);
        SDL_RenderCopy(hud->renderer, hud->texture2, NULL, &hud->rect2);
        SDL_RenderCopy(hud->renderer, hud->texture3, NULL, &hud->rect3);
        SDL_RenderCopy(hud->renderer, hud->texture4, NULL, &hud->rect4);
        SDL_RenderCopy(hud->renderer, hud->texture5, NULL, &hud->rect5);
        SDL_RenderCopy(hud->renderer, hud->texture6, NULL, &hud->rect6);


        SDL_RenderPresent(hud->renderer);
    }
    /* Deinit TTF.*/
    SDL_DestroyTexture(hud->texture1);
    SDL_DestroyTexture(hud->texture2);
    SDL_DestroyTexture(hud->texture3);
    SDL_DestroyTexture(hud->texture4);
    SDL_DestroyTexture(hud->texture5);
    SDL_DestroyTexture(hud->texture6);

    SDL_DestroyRenderer(hud->renderer);
    SDL_DestroyWindow(hud->window);
}


int     main() 
{
    t_hud   *hud;
    hud = malloc(sizeof(t_hud));
    if (hud == NULL)
        exit(EXIT_FAILURE); // make redirect to error output 

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    TTF_Init();
    print_hud(hud, 4, 2, 89);
    TTF_Quit();
    SDL_Quit();
    return (EXIT_SUCCESS);
}

/* gcc hud2.c -o hud2 -I ./frameworks/SDL2.framework/Headers -I ./frameworks/SDL2_ttf.framework/Headers -rpath @loader_path/frameworks -F./frameworks/ -framework SDL2  -framework SDL2_ttf */