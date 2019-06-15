#include "player_stable.h"

/* Moves the player by (dx,dy) in the map, and also updates
 * their anglesin/anglecos/sector properties properly.
 */

//NORMED

void MovePlayer(float dx, float dy, t_player *player, t_sector *sectors)
{
	t_sector* sect;
	float px;
	float py;
	int   s;

	px = player->where.x;
	py = player->where.y;
	sect = &sectors[player->num_sect];
	s = -1;
	while (++s < sect->npoints)
	{
		if (sect->neighbors[s] >= 0 && IntersectBox(px, py, px+dx, py+dy,
			sect->vertex[s+0].x, sect->vertex[s+0].y, sect->vertex[s+1].x,
			sect->vertex[s+1].y) && PointSide(px+dx, py+dy, sect->vertex[s+0].x,
			sect->vertex[s+0].y, sect->vertex[s+1].x, sect->vertex[s+1].y) < 0)
		{
			player->num_sect = sect->neighbors[s];
			break;
		}
	}
	player->where.x += dx;
	player->where.y += dy;
	player->anglesin = sinf(player->angle);
	player->anglecos = cosf(player->angle);
}