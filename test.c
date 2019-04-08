#include <SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    argc = 0;
    argv = 0;

    SDL_Window *window;                     // Declare a pointer
    SDL_Event   event;
    const Uint8     *keystate;
    keystate = SDL_GetKeyboardState(NULL);
                        

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    int is_running = 1;
  //  SDL_Surface     *surface = SDL_GetWindowSurface(window);
    
    while (is_running)
    {          
        if (SDL_PollEvent(&event))
                if (event.type == SDL_QUIT ||
                keystate[SDL_SCANCODE_ESCAPE])
                    is_running = 0;
        // The window is open: could enter program loop here (see SDL_PollEvent())
        SDL_UpdateWindowSurface(window);
    }
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // // Clean up
    SDL_Quit();
    return 0;
}

// gcc test.c -I ./frameworks/SDL2.framework/Headers -I ./frameworks/SDL2_ttf.framework/Headers -I ./frameworks/SDL2_image.framework/Headers -rpath @loader_path/frameworks -F./frameworks/ -framework SDL2 -framework SDL2_image -framework SDL2_ttf