#include "stdlib.h"
#include "player.h"

void        UnloadData(unsigned *NumSectors, t_sector *sectors)
{
    for(unsigned a = 0; a < *NumSectors; ++a)
        free(sectors[a].vertex), free(sectors[a].neighbors);
    free(sectors);
//    *sectors = NULL; //may not work
    *NumSectors = 0;
}
