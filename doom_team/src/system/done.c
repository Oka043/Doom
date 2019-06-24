#include "player_stable.h"

int     done(t_system *system)
{
    UnloadData(&system->NumSectors, system->sectors);
    SDL_Quit();
    return (0);
}