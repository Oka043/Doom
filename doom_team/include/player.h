#ifndef DOOM_NUKEM_PLAYER_H
#define DOOM_NUKEM_PLAYER_H

//# include "ft_math.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
**** USER LIBRARY ****
*/

#include "window.h"

/* Define camera height from floor and distance to ceiling */
#define EyeHeight  6
#define DuckHeight 2.5
#define HeadMargin 1
#define KneeHeight 2 /* Maximum walkable obstacle height */
/* Define factors that affect the field of vision (never >= 180 degrees though) */
#define hfov (1.0 * 0.73f*H/W)
#define vfov (1.0 * .2f)

//#define TextureMapping
//#define DepthShading
//#define LightMapping
//#define VisibilityTracking
//#define SplitScreen

// Utility functions. Because C doesn't have templates,
// we use the slightly less safe preprocessor macros to
// implement these functions that work with multiple types.
#define min(a,b) (((a) < (b)) ? (a) : (b)) // min: Choose smaller of two values.
#define max(a,b) (((a) > (b)) ? (a) : (b)) // max: Choose bigger of two values.
#define abs(a) ((a) < 0 ? -(a) : (a))
#define clamp(a, mi,ma) min(max(a,mi),ma)  // clamp: Clamp value into set range.
#define sign(v) (((v) > 0) - ((v) < 0))    // sign: Return the sign of a value (-1, 0 or 1)
#define vxs(x0,y0, x1,y1) ((x0)*(y1) - (x1)*(y0)) // vxs: Vector cross product
// Overlap:  Determine whether the two number ranges overlap.
#define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
// IntersectBox: Determine whether two 2D-boxes intersect.
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (Overlap(x0,x1,x2,x3) && Overlap(y0,y1,y2,y3))
// PointSide: Determine which side of a line the point is on. Return value: -1, 0 or 1.
#define PointSide(px,py, x0,y0, x1,y1) sign(vxs((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0)))
// Intersect: Calculate the point of intersection between two lines.
#define Intersect(x1,y1, x2,y2, x3,y3, x4,y4) ((t_xy) { \
    vxs(vxs(x1,y1, x2,y2), (x1)-(x2), vxs(x3,y3, x4,y4), (x3)-(x4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)), \
    vxs(vxs(x1,y1, x2,y2), (y1)-(y2), vxs(x3,y3, x4,y4), (y3)-(y4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) })

// Some hard-coded limits.
#define MaxVertices 100 // maximum number of vertices in a map
#define MaxEdges 100    // maximum number of edges in a sector
#define MaxQueue 32     // maximum number of pending portal renders


/* Player: location
** where - Current position
** velocity - Current motion vector
** angle, anglesin,
** 			anglecos, yaw - Looking towards (and sin() and cos() thereof)
** sector - Which sector the player is currently in
**
**
**
**
**
*/


typedef struct s_xy
{
    float x;
    float y;
}               t_xy;

typedef struct      s_sector
{
    float           floor;
    float           ceil;
    t_xy            *vertex;
    /* Each vertex has an x and y coordinate */
    unsigned short  npoints;            /* How many vertexes there are */
    signed char     *neighbours;            /* Each pair of vertexes may have a corresponding neighbouring sector */
#ifdef VisibilityTracking
    int             visible;
#endif
#ifdef TextureMapping
    struct TextureSet *floortexture, *ceiltexture, *uppertextures, *lowertextures;
#endif
}                   t_sector;

typedef struct      s_system
{
    unsigned        NumSectors;
    t_sector        *sectors;
//    SDL_Surface     *surface;
}                   t_system;


typedef struct      s_xyz
{
    float           x;
    float           y;
    float           z;
}                   t_xyz;      /* Current position */

/* Player: location */
typedef struct      s_player
{
    t_xyz           where;
    t_xyz           velocity;   /* Current motion vector */
    float           angle, anglesin, anglecos, yaw;   /* Looking towards (and sin() and cos() thereof) */
    unsigned char   num_sect;                   /* Which sector the player is currently in */
}                   t_player;

typedef struct      s_scaler
{
    int result, bop, fd, ca, cache;
}                   t_scaler;


void        UnloadData(unsigned *NumSectors, t_sector *sectors);
t_sector*   LoadData(t_player *player, t_system *system, t_sector *sectors);

int Scaler_Next(t_scaler* i);
void plot(int x,int y, float opacity, int color, t_system *system);
void line(float x0,float y0, float x1,float y1, int color, t_system *system);
void BloomPostprocess(t_system *system);
void fillpolygon(t_sector* sect, int color, t_system *system);
int vert_compare(const t_xy* a, const t_xy* b);
void vline(int x, int y1,int y2, int top,int middle,int bottom);
void MovePlayer(float dx, float dy, t_player *player, t_sector *sectors);
void SaveFrame1(void);
void SaveFrame2(void);

SDL_Surface* DrawScreen(t_player *player, unsigned *NumSectors, SDL_Surface* surface, t_sector *sectors);



#endif
