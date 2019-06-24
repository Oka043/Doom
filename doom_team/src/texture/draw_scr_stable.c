#include "player_stable.h"


void vline(int x, int y1, int y2, int top, int middle, int bottom, t_system *system)
{
	int *pix;
	int y;

	pix = (int*) system->surface->pixels;
	y1 = clamp(y1, 0, H-1);
	y2 = clamp(y2, 0, H-1);
	if (y2 == y1)
		pix[y1*W2+x] = middle;
	else if (y2 > y1)
	{
		pix[y1*W2+x] = top;
		y = y1;
		while (++y < y2)
			pix[y*W2+x] = middle;
		pix[y2*W2+x] = bottom;
	}
}

void    draw_screen_init(t_system *system, int *renderedsectors, t_player *player)
{
	system->dscreen.tail = system->dscreen.queue;
	system->dscreen.head = system->dscreen.queue;
	system->dscreen.x = 0;
	while (++system->dscreen.x < W)
		system->dscreen.ytop[system->dscreen.x] = 0;
	system->dscreen.x = 0;
	while (++system->dscreen.x < W)
		system->dscreen.ybottom[system->dscreen.x] = H - 1;
	system->dscreen.n = -1;
	while (++system->dscreen.n < system->NumSectors)
		renderedsectors[system->dscreen.n] = 0;
	*system->dscreen.head = (t_item) {player->num_sect, 0, W-1 };
	if(++system->dscreen.head == system->dscreen.queue + MAX_QUEUE)
		system->dscreen.head = system->dscreen.queue;
}

void    transform_vertices(t_system *system, t_player *player)
{
	system->dscreen.v1.x = system->dscreen.sect->vertex[system->dscreen.s + 0].x -
	                      player->where.x;
	system->dscreen.v1.y = system->dscreen.sect->vertex[system->dscreen.s + 0].y -
	                      player->where.y;
	system->dscreen.v2.x = system->dscreen.sect->vertex[system->dscreen.s + 1].x -
	                      player->where.x;
	system->dscreen.v2.y = system->dscreen.sect->vertex[system->dscreen.s + 1].y -
	                      player->where.y;
	system->dscreen.pcos = player->anglecos;
	system->dscreen.psin = player->anglesin;
	system->dscreen.tx1 = system->dscreen.v1.x * system->dscreen.psin
			- system->dscreen.v1.y * system->dscreen.pcos;
	system->dscreen.tz1 = system->dscreen.v1.x * system->dscreen.pcos
			+ system->dscreen.v1.y * system->dscreen.psin;
	system->dscreen.tx2 = system->dscreen.v2.x * system->dscreen.psin
			- system->dscreen.v2.y * system->dscreen.pcos;
	system->dscreen.tz2 = system->dscreen.v2.x * system->dscreen.pcos
			+ system->dscreen.v2.y * system->dscreen.psin;
}

void    view_frustrum_next(t_system *system)
{
	if (system->dscreen.tz1 < system->dscreen.nearz)
	{
		if (system->dscreen.i1.y > 0)
		{
			system->dscreen.tx1 = system->dscreen.i1.x;
			system->dscreen.tz1 = system->dscreen.i1.y;
		} else
		{
			system->dscreen.tx1 = system->dscreen.i2.x;
			system->dscreen.tz1 = system->dscreen.i2.y;
		}
	}
	if (system->dscreen.tz2 < system->dscreen.nearz)
	{
		if (system->dscreen.i1.y > 0)
		{
			system->dscreen.tx2 = system->dscreen.i1.x;
			system->dscreen.tz2 = system->dscreen.i1.y;
		} else
		{
			system->dscreen.tx2 = system->dscreen.i2.x;
			system->dscreen.tz2 = system->dscreen.i2.y;
		}
	}
}

void    view_frustrum(t_system *system)
{
	if (system->dscreen.tz1 <= 0 || system->dscreen.tz2 <= 0)
	{
		system->dscreen.nearz = 1e-4f;
		system->dscreen.farz = 5;
		system->dscreen.nearside = 1e-5f;
		system->dscreen.farside = 20.f;
		system->dscreen.i1 = Intersect(system->dscreen.tx1, system->dscreen.tz1,
				system->dscreen.tx2, system->dscreen.tz2, -system->dscreen.nearside,
				system->dscreen.nearz, -system->dscreen.farside, system->dscreen.farz);
		system->dscreen.i2 = Intersect(system->dscreen.tx1, system->dscreen.tz1,
				system->dscreen.tx2, system->dscreen.tz2, system->dscreen.nearside,
				system->dscreen.nearz, system->dscreen.farside, system->dscreen.farz);
		view_frustrum_next(system);
	}
}

void    scales(t_system *system)
{
	system->dscreen.scale1.x = (W * hfov) / (system->dscreen.tz1);
	system->dscreen.scale1.y = (H * vfov) / (system->dscreen.tz1);
	system->dscreen.x1 = W / 2 + (int) (-system->dscreen.tx1
			* system->dscreen.scale1.x);
	system->dscreen.scale2.x = (W * hfov) / (system->dscreen.tz2);
	system->dscreen.scale2.y = (H * vfov) / (system->dscreen.tz2);
	system->dscreen.x2 = W / 2 + (int) (-system->dscreen.tx2
			* system->dscreen.scale2.x);
}

void    transform_floor_ceiling(t_system *system, t_player *player)
{
	system->dscreen.yceil = system->dscreen.sect->ceil - player->where.z;
	system->dscreen.yfloor = system->dscreen.sect->floor - player->where.z;
	system->dscreen.y1.a = H / 2 + (int) (-Yaw(system->dscreen.yceil,
			system->dscreen.tz1) * system->dscreen.scale1.y),
			system->dscreen.y1.b = H / 2 + (int) (-Yaw(system->dscreen.yfloor,
					system->dscreen.tz1) * system->dscreen.scale1.y);
	system->dscreen.y2.a = H / 2 + (int) (-Yaw(system->dscreen.yceil,
			system->dscreen.tz2) * system->dscreen.scale2.y);
	system->dscreen.y2.b = H / 2 + (int) (-Yaw(system->dscreen.yfloor,
			system->dscreen.tz2) * system->dscreen.scale2.y);
}

void    check_the_edge_type(t_system *system, t_player *player)
{
	system->dscreen.neighbour = system->dscreen.sect->neighbours[system->dscreen.s];
	system->dscreen.nyceil = 0;
	system->dscreen.nyfloor = 0;
	if (system->dscreen.neighbour >= 0)
	{
		system->dscreen.nyceil = system->sectors[system->dscreen.neighbour].ceil
				- player->where.z;
		system->dscreen.nyfloor = system->sectors[system->dscreen.neighbour].floor
				- player->where.z;
	}
	system->dscreen.ny1.a = H / 2 + (int) (-Yaw(system->dscreen.nyceil,
			system->dscreen.tz1) * system->dscreen.scale1.y);
	system->dscreen.ny1.b = H / 2 + (int) (-Yaw(system->dscreen.nyfloor,
			system->dscreen.tz1) * system->dscreen.scale1.y);
	system->dscreen.ny2.a = H / 2 + (int) (-Yaw(system->dscreen.nyceil,
			system->dscreen.tz2) * system->dscreen.scale2.y);
	system->dscreen.ny2.b = H / 2 + (int) (-Yaw(system->dscreen.nyfloor,
			system->dscreen.tz2) * system->dscreen.scale2.y);
}

void    acquire_coords(t_system *system)
{
	system->dscreen.z = ((system->dscreen.xrw - system->dscreen.x1)
			* (system->dscreen.tz2 - system->dscreen.tz1) /
			(system->dscreen.x2 - system->dscreen.x1) + system->dscreen.tz1) * 8;
	system->dscreen.y.a = (system->dscreen.xrw - system->dscreen.x1) *
			(system->dscreen.y2.a - system->dscreen.y1.a) /
			(system->dscreen.x2 - system->dscreen.x1) + system->dscreen.y1.a;
	system->dscreen.y.b = (system->dscreen.xrw - system->dscreen.x1) *
			(system->dscreen.y2.b - system->dscreen.y1.b) /
			(system->dscreen.x2 - system->dscreen.x1) + system->dscreen.y1.b;
}

void    clamp_ya_yb(t_system *system)
{
	system->dscreen.cy.a = clamp(system->dscreen.y.a,
			system->dscreen.ytop[system->dscreen.xrw],
			system->dscreen.ybottom[system->dscreen.xrw]);
	system->dscreen.cy.b = clamp(system->dscreen.y.b,
			system->dscreen.ytop[system->dscreen.xrw],
			system->dscreen.ybottom[system->dscreen.xrw]);
	vline (system->dscreen.xrw, system->dscreen.ytop[system->dscreen.xrw],
			system->dscreen.cy.a - 1, 0x111111, 0x222222, 0x111111, system);
	vline (system->dscreen.xrw, system->dscreen.cy.b + 1,
			system->dscreen.ybottom[system->dscreen.xrw], 0x0000FF, 0x0000AA,
			0x0000FF, system);
}

void    render_upper_wall(t_system *system)
{
	system->dscreen.r2 = 0x040007 * (31 - system->dscreen.z / 8);
	system->dscreen.r1 = 0x010101 * (255 - system->dscreen.z);
	vline (system->dscreen.xrw, system->dscreen.cy.a,
			system->dscreen.cny.a - 1, 0,
			system->dscreen.xrw == system->dscreen.x1 ||
			system->dscreen.xrw == system->dscreen.x2 ?
			0 : system->dscreen.r1, 0, system);
	system->dscreen.ytop[system->dscreen.xrw] = clamp(max (system->dscreen.cy.a,
			system->dscreen.cny.a), system->dscreen.ytop[system->dscreen.xrw],
					H - 1);
	vline (system->dscreen.xrw, system->dscreen.cny.b + 1,
			system->dscreen.cy.b, 0, system->dscreen.xrw == system->dscreen.x1 ||
			system->dscreen.xrw == system->dscreen.x2 ?
			0 : system->dscreen.r2, 0, system);
	system->dscreen.ybottom[system->dscreen.xrw] = clamp(min
			(system->dscreen.cy.b, system->dscreen.cny.b), 0,
			system->dscreen.ybottom[system->dscreen.xrw]);
}

void    another_sector(t_system *system)
{
	system->dscreen.ny.a = (system->dscreen.xrw - system->dscreen.x1) *
			(system->dscreen.ny2.a - system->dscreen.ny1.a) /
			(system->dscreen.x2 - system->dscreen.x1) + system->dscreen.ny1.a;
	system->dscreen.ny.b = (system->dscreen.xrw - system->dscreen.x1) *
			(system->dscreen.ny2.b - system->dscreen.ny1.b) /
			(system->dscreen.x2 - system->dscreen.x1) + system->dscreen.ny1.b;
	if (system->dscreen.neighbour >= 0)
	{
		system->dscreen.cny.a = clamp(system->dscreen.ny.a,
				system->dscreen.ytop[system->dscreen.xrw],
				system->dscreen.ybottom[system->dscreen.xrw]);
		system->dscreen.cny.b = clamp(system->dscreen.ny.b,
				system->dscreen.ytop[system->dscreen.xrw],
				system->dscreen.ybottom[system->dscreen.xrw]);
		render_upper_wall(system);
	}
	else
	{
		system->dscreen.r = 0x010101 * (255 - system->dscreen.z);
		vline (system->dscreen.xrw, system->dscreen.cy.a, system->dscreen.cy.b, 0,
				system->dscreen.xrw == system->dscreen.x1 ||
				system->dscreen.xrw == system->dscreen.x2 ?
				0 : system->dscreen.r, 0, system);
	}
}

void    render_wall(t_system* system)
{
	system->dscreen.beginx = max(system->dscreen.x1, system->dscreen.now.sx1);
	system->dscreen.endx = min(system->dscreen.x2, system->dscreen.now.sx2);
	system->dscreen.xrw = system->dscreen.beginx - 1;
	while (++system->dscreen.xrw <= system->dscreen.endx)
	{
		acquire_coords(system);
		clamp_ya_yb(system);
		another_sector(system);
	}
}

void    schedule_sector(t_system *system)
{
	if (system->dscreen.neighbour >= 0 &&
		system->dscreen.endx >= system->dscreen.beginx &&
	    (system->dscreen.head + MAX_QUEUE +
	    1 - system->dscreen.tail) % MAX_QUEUE)
	{
		*system->dscreen.head = (t_item) {system->dscreen.neighbour,
				system->dscreen.beginx, system->dscreen.endx};
		if (++system->dscreen.head == system->dscreen.queue + MAX_QUEUE)
			system->dscreen.head = system->dscreen.queue;
	}
}

void    render_each_wall(t_system *system, t_player *player)
{
	system->dscreen.sect = &system->sectors[system->dscreen.now.sectorno];
	system->dscreen.s = -1;
	while (++system->dscreen.s < system->dscreen.sect->npoints)
	{
		transform_vertices(system, player);
		if (system->dscreen.tz1 <= 0 && system->dscreen.tz2 <= 0)
			continue;
		view_frustrum(system);
		scales(system);
		if (system->dscreen.x1 >= system->dscreen.x2 ||
		system->dscreen.x2 < system->dscreen.now.sx1 ||
		    system->dscreen.x1 > system->dscreen.now.sx2)
			continue;
		transform_floor_ceiling(system, player);
		check_the_edge_type(system, player);
		render_wall(system);
		schedule_sector(system);
	}
}

//		Есть вероятность что может пригодится
//void DrawScreen(t_player *player, t_system *system)
//{
//	int renderedsectors[system->NumSectors];
//
//	draw_screen_init(system, renderedsectors, player);
//    SDL_LockSurface(system->surface);
//    while (system->dscreen.head != system->dscreen.tail)
//    {
//	    system->dscreen.now = *system->dscreen.tail;
//	    if (++system->dscreen.tail == system->dscreen.queue + MAX_QUEUE)
//		    system->dscreen.tail = system->dscreen.queue;
//	    if (renderedsectors[system->dscreen.now.sectorno] & 0x21)
//		    continue;
//	    ++renderedsectors[system->dscreen.now.sectorno];
//	    render_each_wall (system, player);
//	    ++renderedsectors[system->dscreen.now.sectorno];
//    }
//    SDL_UnlockSurface(system->surface);
//    SaveFrame2(system);
//}