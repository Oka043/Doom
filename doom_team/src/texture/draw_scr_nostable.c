#include "player_stable.h"

# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

void vline2(int x, int y1, int y2, struct Scaler ty, unsigned txtx, const struct TextureSet* t, t_system *system)
{
	int *pix = (int*) system->surface->pixels;
	int y;

	y1 = clamp(y1, 0, H-1);
	y2 = clamp(y2, 0, H-1);
	pix += y1 * W2 + x;
	y = y1;
	while (y <= y2)
	{
		unsigned txty = Scaler_Next(&ty);
		*pix = t->texture[txtx % 64][txty % 64];
		pix += W2;
		y++;
	}
}

void    DrawScreen(t_player *player, t_system *system)
{
	int renderedsectors[system->NumSectors];

	draw_screen_init(system, renderedsectors, player);
	SDL_LockSurface(system->surface);

	while (SDSC.head != SDSC.tail)
	{
		SDSC.now = *SDSC.tail;
		if (++SDSC.tail == SDSC.queue + MAX_QUEUE)
			SDSC.tail = SDSC.queue;
		if (renderedsectors[SDSC.now.sectorno] & 0x21)
			continue;
		++renderedsectors[SDSC.now.sectorno];

		SDSC.sect = &SS[SDSC.now.sectorno];
		SDSC.s = -1;
		while (++SDSC.s < SDSC.sect->npoints)
		{
			transform_vertices(system, player);
			/* Is the wall at least partially in front of the player? */
			if (SDSC.tz1 <= 0 && SDSC.tz2 <= 0)
				continue;
			int u0;
			int u1;

			u0 = 0;
			u1 = 127;
			if (SDSC.tz1 <= 0 || SDSC.tz2 <= 0)
			{
				SDSC.nearz = 1e-4f;
				SDSC.farz = 5;
				SDSC.nearside = 1e-5f;
				SDSC.farside = 20.f;
				SDSC.i1 = Intersect(SDSC.tx1, SDSC.tz1,
									SDSC.tx2, SDSC.tz2, -SDSC.nearside,
									SDSC.nearz, -SDSC.farside, SDSC.farz);
				SDSC.i2 = Intersect(SDSC.tx1, SDSC.tz1,
									SDSC.tx2, SDSC.tz2, SDSC.nearside,
									SDSC.nearz, SDSC.farside, SDSC.farz);
				t_xy	org1 = {SDSC.tx1,SDSC.tz1};
				t_xy	org2 = {SDSC.tx2,SDSC.tz2};

				view_frustrum_next(system);

				if (abs(SDSC.tx2-SDSC.tx1) > abs(SDSC.tz2-SDSC.tz1))
				{
					u0 = (int)((SDSC.tx1-org1.x) * 127 / (org2.x-org1.x));
					u1 = (int)((SDSC.tx2-org1.x) * 127 / (org2.x-org1.x));
				}
				else
				{
					u0 = (int)((SDSC.tz1-org1.y) * 127 / (org2.y-org1.y));
					u1 = (int)((SDSC.tz2-org1.y) * 127 / (org2.y-org1.y));
				}
			}
			scales(system);
			if (SDSC.x1 >= SDSC.x2 || SDSC.x2 < SDSC.now.sx1 || SDSC.x1 > SDSC.now.sx2)
				continue;

			transform_floor_ceiling(system, player);
			check_the_edge_type(system, player);

			SDSC.beginx = max(SDSC.x1, SDSC.now.sx1);
			SDSC.endx = min(SDSC.x2, SDSC.now.sx2);

			t_scaler nya_int = Scaler_Init(SDSC.x1,SDSC.beginx,SDSC.x2, SDSC.ny1.a,SDSC.ny2.a);
			t_scaler nyb_int = Scaler_Init(SDSC.x1,SDSC.beginx,SDSC.x2, SDSC.ny1.b,SDSC.ny2.b);

			SDSC.xrw = SDSC.beginx - 1;

			while (++SDSC.xrw <= SDSC.endx)
			{
				int txtx = (u0*((SDSC.x2-SDSC.xrw)*SDSC.tz2)
							+ u1*((SDSC.xrw-SDSC.x1)*SDSC.tz1))
						   / ((SDSC.x2-SDSC.xrw)*SDSC.tz2
							  + (SDSC.xrw-SDSC.x1)*SDSC.tz1);

				acquire_coords(system);

				SDSC.cy.a = clamp(SDSC.y.a,
								  SDSC.ytop[SDSC.xrw],
								  SDSC.ybottom[SDSC.xrw]);
				SDSC.cy.b = clamp(SDSC.y.b,
								  SDSC.ytop[SDSC.xrw],
								  SDSC.ybottom[SDSC.xrw]);



				for(int y=SDSC.ytop[SDSC.xrw]; y<=SDSC.ybottom[SDSC.xrw]; ++y)
				{
					if(y >= SDSC.cy.a && y <= SDSC.cy.b)
					{
						y = SDSC.cy.b;
						continue;
					}
					float hei = y < SDSC.cy.a ? SDSC.yceil    : SDSC.yfloor;
					float mapx, mapz;

					mapz = (hei)*H*vfov / ((H/2 - (y)) - player->yaw*H*vfov);
					mapx = (mapz) * (W/2 - (SDSC.xrw)) / (W*hfov);

					float rtx = (mapz) * player->anglecos + (mapx) * player->anglesin;
					float rtz = (mapz) * player->anglesin - (mapx) * player->anglecos;
					mapx = rtx + player->where.x; mapz = rtz + player->where.y;


					unsigned txtx = (mapx * 63), txtz = (mapz * 63);
					struct TextureSet* txt;
					txt = y < SDSC.cy.a ? SDSC.sect->ceiltexture
										: SDSC.sect->floortexture;

					int pel = txt->texture[txtz % 64][txtx % 64];
					((int*)system->surface->pixels)[y*W2+SDSC.xrw] = pel;
				}

				if (SDSC.neighbour >= 0)
				{
					SDSC.ny.a = Scaler_Next (&nya_int);
					SDSC.ny.b = Scaler_Next (&nyb_int);
					SDSC.cny.a = clamp(SDSC.ny.a, SDSC.ytop[SDSC.xrw], SDSC.ybottom[SDSC.xrw]);
					SDSC.cny.b = clamp(SDSC.ny.b, SDSC.ytop[SDSC.xrw], SDSC.ybottom[SDSC.xrw]);

					vline2(SDSC.xrw, SDSC.cy.a, SDSC.cny.a-1,
						   (struct Scaler)Scaler_Init(SDSC.y.a,SDSC.cy.a, SDSC.y.b, 0,63),
						   txtx, &SDSC.sect->uppertextures[SDSC.s], system);
					SDSC.ytop[SDSC.xrw] = clamp(max(SDSC.cy.a, SDSC.cny.a),
												SDSC.ytop[SDSC.xrw], H-1);

					vline2(SDSC.xrw, SDSC.cny.b+1, SDSC.cy.b,
						   (struct Scaler)Scaler_Init(SDSC.y.a, SDSC.cny.b+1,SDSC.y.b, 0,63),
						   txtx, &SDSC.sect->lowertextures[SDSC.s], system);

					SDSC.ybottom[SDSC.xrw] = clamp(min(SDSC.cy.b, SDSC.cny.b),
												   0, SDSC.ybottom[SDSC.xrw]);
				}
				else
				{
					vline2(SDSC.xrw, SDSC.cy.a, SDSC.cy.b,
						   (struct Scaler)Scaler_Init(SDSC.y.a,SDSC.cy.a,SDSC.y.b, 0, 127),
						   txtx, &SDSC.sect->uppertextures[SDSC.s], system);
				}

				/* // uncomment this to see the process of a frame being rendered in real time.
				SDL_UnlockSurface(surface);
				save_frame2(surface); SDL_Flip(surface);
				SDL_LockSurface(surface);*/
			}
			if (SDSC.neighbour >= 0 &&
				SDSC.endx >= SDSC.beginx &&
				(SDSC.head + MAX_QUEUE +
				 1 - SDSC.tail) % MAX_QUEUE)
			{
				*SDSC.head = (t_item) {SDSC.neighbour,
									   SDSC.beginx, SDSC.endx};
				if (++SDSC.head == SDSC.queue + MAX_QUEUE)
					SDSC.head = SDSC.queue;
			}
		}
		++renderedsectors[SDSC.now.sectorno];
	}

	SDL_UnlockSurface(system->surface);
	save_frame2(system);
}