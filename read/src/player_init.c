#include "player_stable.h"

void init_player(t_player *player)
{
	PP.wsad[0] = 0;
	PP.wsad[1] = 0;
	PP.wsad[2] = 0;
	PP.wsad[3] = 0;
	PP.ground = 0;
	PP.falling = 1;
	PP.moving = 0;
	PP.ducking = 0;
	PP.map = 0;
}