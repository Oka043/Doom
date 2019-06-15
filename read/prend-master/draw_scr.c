#include "player.h"

SDL_Surface*  DrawScreen(t_player *player, unsigned *NumSectors, SDL_Surface *surface, t_sector *sectors)
{
    struct item { short sectorno,sx1,sx2; } queue[MaxQueue], *head=queue, *tail=queue;
    short ytop[W]={0}, ybottom[W], renderedsectors[*NumSectors];
    for(unsigned x=0; x<W; ++x) ybottom[x] = H-1;
    for(unsigned n=0; n < *NumSectors; ++n)
        renderedsectors[n] = 0;

#ifdef VisibilityTracking
    for(unsigned n=0; n<system->NumSectors; ++n) sectors[n].visible=0;
#endif
#ifdef VisibilityTracking
    memset(VisibleFloors, 0, sizeof(VisibleFloors));
    memset(VisibleCeils, 0, sizeof(VisibleCeils));
    NumVisibleSectors=0;
#endif

    /*Begin whole-screen rendering from where the player is. */
    *head = (struct item) { player->num_sect, 0, W-1 };
    if(++head == queue+MaxQueue) head = queue;

    SDL_LockSurface(surface);

    while(head != tail)
    {
        /* Pick a sector & slice from queue to draw */
        const struct item now = *tail;
        if(++tail == queue+MaxQueue) tail = queue;

        if(renderedsectors[now.sectorno] & 0x21) continue; // Odd = still rendering, 0x20 = give up
        ++renderedsectors[now.sectorno];
#ifdef VisibilityTracking
        sectors[now.sectorno].visible=1;
#endif
        /* Render each wall of this sector that is facing towards player-> */
        const struct sector* const sect = &sectors[now.sectorno];
#ifdef LightMapping
        struct xy bounding_min = {1e9f, 1e9f}, bounding_max = {-1e9f, -1e9f};
        GetSectorBoundingBox(now.sectorno, &bounding_min, &bounding_max);
#endif

        /* This loop can be used to illustrate currently rendering window. Should be disabled otherwise. */
        //for(unsigned x=now.sx1; x<=now.sx2; ++x)
        //    vline(x, ytop[x], ybottom[x], 0x003300, 0x00AA00, 0x003300);

        for(int s = 0; s < sect->npoints; ++s)
        {
            /* Acquire the x,y coordinates of the two vertexes forming the edge of the sector */
            /* Transform the vertices into the player's view */
            float vx1 = sect->vertex[s+0].x - player->where.x, vy1 = sect->vertex[s+0].y - player->where.y;
            float vx2 = sect->vertex[s+1].x - player->where.x, vy2 = sect->vertex[s+1].y - player->where.y;
            /* Rotate them around the player's view */
            float pcos = player->anglecos, psin = player->anglesin;
            float tx1 = vx1 * psin - vy1 * pcos,  tz1 = vx1 * pcos + vy1 * psin;
            float tx2 = vx2 * psin - vy2 * pcos,  tz2 = vx2 * pcos + vy2 * psin;
            /* Is the wall at least partially in front of the player? */
            if(tz1 <= 0 && tz2 <= 0) continue;
            /* If it's partially behind the player, clip it against player's view frustrum */
#ifdef TextureMapping
            int u0 = 0, u1 = 1023;
#endif
            if(tz1 <= 0 || tz2 <= 0)
            {
                float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
                // Find an intersetion between the wall and the approximate edges of player's view
                t_xy i1 = Intersect(tx1,tz1,tx2,tz2, -nearside,nearz, -farside,farz);
                t_xy i2 = Intersect(tx1,tz1,tx2,tz2,  nearside,nearz,  farside,farz);
#ifdef TextureMapping
                struct xy org1 = {tx1,tz1}, org2 = {tx2,tz2};
#endif
                if(tz1 < nearz) { if(i1.y > 0) { tx1 = i1.x; tz1 = i1.y; } else { tx1 = i2.x; tz1 = i2.y; } }
                if(tz2 < nearz) { if(i1.y > 0) { tx2 = i1.x; tz2 = i1.y; } else { tx2 = i2.x; tz2 = i2.y; } }
#ifdef TextureMapping
                if(abs(tx2-tx1) > abs(tz2-tz1))
                    u0 = (tx1-org1.x) * 1023 / (org2.x-org1.x), u1 = (tx2-org1.x) * 1023 / (org2.x-org1.x);
                else
                    u0 = (tz1-org1.y) * 1023 / (org2.y-org1.y), u1 = (tz2-org1.y) * 1023 / (org2.y-org1.y);
#endif
            }

            // if(tz1 <= 0.07f) { tx1 = (0.07f-tz1) * (tx2-tx1) / (tz2-tz1) + tx1; tz1 = 0.07f; }
            // if(tz2 <= 0.07f) { tx2 = (0.07f-tz2) * (tx1-tx2) / (tz1-tz2) + tx2; tz2 = 0.07f; }
            /* Do perspective transformation */
            float xscale1 = (W*hfov) / (tz1), yscale1 = (H*vfov) / (tz1); int x1 = W/2 + (int)(-tx1 * xscale1);
            float xscale2 = (W*hfov) / (tz2), yscale2 = (H*vfov) / (tz2); int x2 = W/2 + (int)(-tx2 * xscale2);
            if(x1 >= x2) continue;
            if(x2 < now.sx1 || x1 > now.sx2) continue;
            /* Acquire and transform the floor and ceiling heights */
            float yceil  = sect->ceil  - player->where.z;
            float yfloor = sect->floor - player->where.z;
#define Yaw(y,z) (y + z*player->yaw)
            int y1a = H/2 + (int)(-Yaw(yceil, tz1) * yscale1), y1b = H/2 + (int)(-Yaw(yfloor, tz1) * yscale1);
            int y2a = H/2 + (int)(-Yaw(yceil, tz2) * yscale2), y2b = H/2 + (int)(-Yaw(yfloor, tz2) * yscale2);
            /* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
            int neighbor = sect->neighbors[s];
            float nyceil=0, nyfloor=0;

            if(neighbor >= 0)
            {
                /* Something is showing through this wall (portal). */
                /* Perspective-transform the floor and ceiling coordinates of the neighboring sector. */
                nyceil  = sectors[neighbor].ceil  - player->where.z;
                nyfloor = sectors[neighbor].floor - player->where.z;
            }
            int ny1a = H/2 + (int)( -Yaw(nyceil, tz1) * yscale1), ny1b = H/2 + (int)( -Yaw(nyfloor, tz1) * yscale1);
            int ny2a = H/2 + (int)( -Yaw(nyceil, tz2) * yscale2), ny2b = H/2 + (int)( -Yaw(nyfloor, tz2) * yscale2);

            /* Render the wall. */
            int beginx = max(x1, now.sx1), endx = min(x2, now.sx2);

#if defined(DepthShading) && !defined(TextureMapping)
            struct Scaler z_int  = Scaler_Init(x1,beginx,x2, tz1*8,tz2*8);
#endif
            t_scaler ya_int = Scaler_Init(x1,beginx,x2, y1a,y2a);
            t_scaler yb_int = Scaler_Init(x1,beginx,x2, y1b,y2b);
            t_scaler nya_int = Scaler_Init(x1,beginx,x2, ny1a,ny2a);
            t_scaler nyb_int = Scaler_Init(x1,beginx,x2, ny1b,ny2b);

            for(int x = beginx; x <= endx; ++x)
            {
                // Affine calculation of txtx would be:
                //    alpha : (x-x1) / (x2-x1)
                //    u0    : 0;
                //    u1    : 1023;
                //    txtx  : u0 + u1 * alpha;
                // Perspective-corrected calculation of txtx (from Wikipedia):
                //    txtx  : ((1-alpha) * (u0 / z0) + alpha * (u1 / z1))
                //          / (((1-alpha) / z0) + alpha/z1);
                // Unrolled for only one division with Maxima:
                //    txtx  : u0*z1*(x2-x) + u1*z0*(x-x1)
                //          / ((x2-x)*z1 + (x-x1)*z0);
#ifdef TextureMapping
                int txtx = (u0*((x2-x)*tz2) + u1*((x-x1)*tz1)) / ((x2-x)*tz2 + (x-x1)*tz1);
#endif
#if defined(DepthShading) && !defined(TextureMapping)
                /* Calculate the Z coordinate for this point. (Only used for lighting.) */
                int z = Scaler_Next(&z_int);
#endif
                /* Acquire the Y coordinates for our floor & ceiling for this X coordinate */
                int ya = Scaler_Next(&ya_int);
                int yb = Scaler_Next(&yb_int);
                /* Clamp the ya & yb */
                int cya = clamp(ya, ytop[x],ybottom[x]);
                int cyb = clamp(yb, ytop[x],ybottom[x]);

                // Our perspective calculation produces these two:
                //     screenX = W/2 + -mapX              * (W*hfov) / mapZ
                //     screenY = H/2 + -(mapY + mapZ*yaw) * (H*vfov) / mapZ
                // To translate these coordinates back into mapX, mapY and mapZ...
                //
                // Solving for Z, when we know Y (ceiling height):
                //     screenY - H/2  = -(mapY + mapZ*yaw) * (H*vfov) / mapZ
                //     (screenY - H/2) / (H*vfov) = -(mapY + mapZ*yaw) / mapZ
                //     (H/2 - screenY) / (H*vfov) = (mapY + mapZ*yaw) / mapZ
                //     mapZ = mapY / ((H/2 - screenY) / (H*vfov) - yaw)
                //     mapZ = mapY*H*vfov / (H/2 - screenY - yaw*H*vfov)
                // Solving for X, when we know Z
                //     mapX = mapZ*(W/2 - screenX) / (W*hfov)
                //
                // This calculation is used for visibility tracking
                //   (the visibility cones in the map)
                // and for floor & ceiling texture mapping.
                //
#define CeilingFloorScreenCoordinatesToMapCoordinates(mapY, screenX,screenY, X,Z) \
                    do { Z = (mapY)*H*vfov / ((H/2 - (screenY)) - player->yaw*H*vfov); \
                         X = (Z) * (W/2 - (screenX)) / (W*hfov); \
                         RelativeMapCoordinatesToAbsoluteOnes(X,Z); } while(0)
                //
#define RelativeMapCoordinatesToAbsoluteOnes(X,Z) \
                    do { float rtx = (Z) * pcos + (X) * psin; \
                         float rtz = (Z) * psin - (X) * pcos; \
                         X = rtx + player->where.x; Z = rtz + player->where.y; \
                    } while(0)

#ifdef TextureMapping
                // Texture-mapping for floors and ceilings is not very optimal in my program.
                // I'm converting each screen-pixel into map-coordinates by doing the perspective
                // transformation in reverse, and using these map-coordinates as indexes into texture.
                // This involves a few division calculations _per_ pixel, and would have been way
                // too slow for the platforms targeted by Doom and Duke3D.
                // In any case, there's no neat way to do it.
                // It is why the SNES port of Doom didn't do floor & ceiling textures at all.
                for(int y=ytop[x]; y<=ybottom[x]; ++y)
                {
                    if(y >= cya && y <= cyb) { y = cyb; continue; }
                    float hei = y < cya ? yceil    : yfloor;
                    float mapx, mapz;
                    CeilingFloorScreenCoordinatesToMapCoordinates(hei, x,y,  mapx,mapz);
                    unsigned txtx = (mapx * 256), txtz = (mapz * 256);
                    const struct TextureSet* txt = y < cya ? sect->ceiltexture : sect->floortexture;
  #ifdef LightMapping
                    unsigned lu = ((unsigned)((mapx - bounding_min.x) * 1024 / (bounding_max.x - bounding_min.x))) % 1024;
                    unsigned lv = ((unsigned)((mapz - bounding_min.y) * 1024 / (bounding_max.y - bounding_min.y))) % 1024;
                    int pel = ApplyLight( txt->texture[txtx % 1024][txtz % 1024],
                                          txt->lightmap[lu][lv] );
  #else
                    int pel = txt->texture[txtz % 1024][txtx % 1024];
  #endif
                    ((int*)surface->pixels)[y*W2+x] = pel;
                }
#else
                /* Render ceiling: everything above this sector's ceiling height. */
                vline(x, ytop[x], cya-1, 0x111111 ,0x222222,0x111111);
                /* Render floor: everything below this sector's floor height. */
                vline(x, cyb+1, ybottom[x], 0x0000FF,0x0000AA,0x0000FF);
#endif

#ifdef VisibilityTracking
                // Keep track of what the player can see for a neat map gimmick.
               {unsigned n = NumVisibleSectors;
                if(ybottom[x] >= (cyb+1))
                {
                 float FloorXbegin,FloorZbegin,FloorXend,FloorZend;
                 CeilingFloorScreenCoordinatesToMapCoordinates(yfloor, x,cyb+1,      FloorXbegin,FloorZbegin);
                 CeilingFloorScreenCoordinatesToMapCoordinates(yfloor, x,ybottom[x], FloorXend,  FloorZend);
                 VisibleFloorBegins[n][x] = (struct xy){FloorXbegin,FloorZbegin};
                 VisibleFloorEnds[n][x] = (struct xy){FloorXend,FloorZend};
                 VisibleFloors[n][x] = 1;
                }
                if((cya-1) >= ytop[x])
                {
                 float CeilXbegin, CeilZbegin, CeilXend, CeilZend;
                 CeilingFloorScreenCoordinatesToMapCoordinates(yceil, x,ytop[x],     CeilXbegin,CeilZbegin);
                 CeilingFloorScreenCoordinatesToMapCoordinates(yceil, x,cya-1,       CeilXend,  CeilZend);
                 VisibleCeilBegins[n][x] = (struct xy){CeilXbegin,CeilZbegin};
                 VisibleCeilEnds[n][x] = (struct xy){CeilXend,CeilZend};
                 VisibleCeils[n][x] = 1;
                }
               }
#endif

                /* Is there another sector behind this edge? */
                if(neighbor >= 0)
                {
                    /* Same for _their_ floor and ceiling */
                    int nya = Scaler_Next(&nya_int);
                    int nyb = Scaler_Next(&nyb_int);
                    /* Clamp ya2 & yb2 */
                    int cnya = clamp(nya, ytop[x],ybottom[x]);
                    int cnyb = clamp(nyb, ytop[x],ybottom[x]);
                    /* If our ceiling is higher than their ceiling, render upper wall */
#ifdef TextureMapping
                    vline2(x, cya, cnya-1, (struct Scaler)Scaler_Init(ya,cya,yb, 0,1023), txtx, &sect->uppertextures[s]);
#else
#ifdef DepthShading
                    unsigned r1 = 0x010101 * (255-z), r2 = 0x040007 * (31-z/8);
#else
                    unsigned r1 = 0xAAAAAA, r2 = 0x7C00D9;
#endif
                    vline(x, cya, cnya-1, 0, x==x1||x==x2 ? 0 : r1, 0);
#endif
                    ytop[x] = clamp(max(cya, cnya), ytop[x], H-1);

                    // If our floor is lower than their floor, render bottom wall
#ifdef TextureMapping
                    vline2(x, cnyb+1, cyb, (struct Scaler)Scaler_Init(ya,cnyb+1,yb, 0,1023), txtx, &sect->lowertextures[s]);
#else
                    vline(x, cnyb+1, cyb, 0, x==x1||x==x2 ? 0 : r2, 0);
#endif
                    ybottom[x] = clamp(min(cyb, cnyb), 0, ybottom[x]);

                    /* These vlines can be used to illustrate the frame being rendered. */
                    /* They should be disabled otherwise. */
                    //vline(x, ytop[x],ybottom[x], 0x330000,0xAA0000,0x330000);
                    //vline(x, cya,cyb, 0x330000,0xAA0000,0x330000);
                }
                else
                {
                    /* There's no neighbor. Render wall. */
#ifdef TextureMapping
                    vline2(x, cya,cyb, (struct Scaler)Scaler_Init(ya,cya,yb, 0,1023), txtx, &sect->uppertextures[s]);
#else
#ifdef DepthShading
                    unsigned r = 0x010101 * (255-z);
#else
                    unsigned r = 0xAAAAAA;
#endif
                    vline(x, cya, cyb, 0, x==x1||x==x2 ? 0 : r, 0);
#endif
                }

                /* // uncomment this to see the process of a frame being rendered in real time.
                SDL_UnlockSurface(surface);
                surface = SaveFrame2(surface); SDL_Flip(surface);
                SDL_LockSurface(surface);*/

            }
            /* Schedule the other sector for rendering within the window formed by this wall. */
            if(neighbor >= 0 && endx >= beginx && (head+MaxQueue+1-tail)%MaxQueue)
            {
                *head = (struct item) { neighbor, beginx, endx };
                if(++head == queue+MaxQueue) head = queue;
                //if(tail-- == queue) tail = queue+MaxQueue-1;
                //*tail = (struct item) { neighbor, beginx, endx };
            }
        }
        ++renderedsectors[now.sectorno];
#ifdef VisibilityTracking
        NumVisibleSectors += 1;
#endif
    }

    SDL_UnlockSurface(surface);
    surface = SaveFrame2(surface);
    //static unsigned skip=0;
    //if(/*++skip >= 10 && */!map) { skip=0; SDL_Flip(surface); }
    return surface;
}