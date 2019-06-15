#include "stdlib.h"
#include "player_stable.h"

void        UnloadData(unsigned *NumSectors, t_sector *sectors)
{
	int a;

	a = 0;
	while (++a < *NumSectors)
		free(sectors[a].vertex), free(sectors[a].neighbors);
    free(sectors);
//    *sectors = NULL; //may not work
    *NumSectors = 0;
}
