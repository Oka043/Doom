#ifndef DOOM_NUKEM_PLAYER_H
#define DOOM_NUKEM_PLAYER_H

#include <math.h>
//#include <SDL_image.h>
#include <SDL2/SDL.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "get_next_line.h"
#include "parts.h"
//#include "sdl-gpu/include/SDL_gpu.h"

/*
**** USER LIBRARY ****
*/

#include "window.h"

/* Define camera height from floor and distance to ceiling */
#define EyeHeight  6 /* Camera height from floor when standing */
#define DuckHeight 2.5 /* And when crouching */
#define HeadMargin 1 /* How much room there is above camera before the head hits the ceiling */
#define KneeHeight 2 /* How tall obstacles the player can simply walk over without jumping */

/* Define factors that affect the field of vision (never >= 180 degrees though) */

#define hfov (1.0 * 0.73f*H/W) /* Affects the horizontal field of vision */
#define vfov (1.0 * .2f) /* Affects the vertical field of vision */

#define Yaw(y,z) (y + z*player->yaw)

//#define TextureMapping
//#define DepthShading  // и так работает
//#define LightMapping
//#define VisibilityTracking
//#define SplitScreen // не работает ни в каком виде
//
// Utility functions. Because C doesn't have templates,
// we use the slightly less safe preprocessor macros to
// implement these functions that work with multiple types.
#define min(a,b) (((a) < (b)) ? (a) : (b)) // min: Choose smaller of two values.
#define max(a,b) (((a) > (b)) ? (a) : (b)) // max: Choose bigger of two values.
#define abs(a) ((a) < 0 ? -(a) : (a))
#define clamp(a, mi,ma) min(max(a,mi),ma)  // clamp: Clamp value into set range.
#define sign(v) (((v) > 0) - ((v) < 0))    // sign: Return the sign of a value (-1, 0 or 1)
#define vxs(x0,y0, x1,y1) ((x0)*(y1) - (x1)*(y0)) // vxs: Vector cross product
// OVERLAP:  Determine whether the two number ranges overlap.
#define OVERLAP(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
// IntersectBox: Determine whether two 2D-boxes intersect.
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (OVERLAP(x0,x1,x2,x3) && OVERLAP(y0,y1,y2,y3))
// PointSide: Determine which side of a line the point is on. Return value: -1, 0 or 1.
#define PointSide(px,py, x0,y0, x1,y1) sign(vxs((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0)))
// Intersect: Calculate the point of intersection between two lines.
#define Intersect(x1,y1, x2,y2, x3,y3, x4,y4) ((t_xy) { \
    vxs(vxs(x1,y1, x2,y2), (x1)-(x2), vxs(x3,y3, x4,y4), (x3)-(x4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)), \
    vxs(vxs(x1,y1, x2,y2), (y1)-(y2), vxs(x3,y3, x4,y4), (y3)-(y4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) })

// Some hard-coded limits.
#define MAX_VERTICLES 100 // maximum number of vertices in a map
#define MAX_EDGES 100    // maximum number of edges in a sector
#define MAX_QUEUE 32     // maximum number of pending portal renders


#define Scaler_Init(a,b,c,d,f) \
    { d + (b-1 - a) * (f-d) / (c-a), ((f<d) ^ (c<a)) ? -1 : 1, \
      abs(f-d), abs(c-a), (int)((b-1-a) * abs(f-d)) % abs(c-a) }

// Helper function for the antialiased line algorithm.
#define fpart(x) ((x) < 0 ? 1 - ((x) - floorf(x)) : (x) - floorf(x))
#define rfpart(x) (1 - fpart(x))

/*
**
**   CUSTOM DEFINES
**
*/

#define  PP player->pos
#define  KEY ev->key.keysym.sym
#define SQ(a) (a*a)
#define PW player->where
#define PV player->velocity
#define SS system->sectors
#define SEV sector->vertex
#define SW system->window


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


typedef struct 	    s_xy
{
	float 		    x;
	float 		    y;
}                   t_xy;

typedef struct      s_ab
{
	int             a;
	int             b;
}                   t_ab;

typedef struct      s_sector
{
	float           floor;
	float           ceil;
	t_xy            *vertex;    /* Each vertex has an x and y coordinate */
	int             npoints;    /* How many vertexes there are */
	signed char     *neighbors; /* Each pair of vertexes may have a corresponding neighboring sector */
}                   t_sector;

typedef struct      s_item
{
	int             sectorno;
	int             sx1;
	int             sx2;
}                   t_item;

typedef struct  s_drawscreen
{
	t_item      item;
	t_item      queue[MAX_QUEUE];
	t_item      *head;
	t_item      *tail;
	t_item      now;
	int         x;
	int         ytop[W];
	int         ybottom[W];
	int         n;
	t_sector*   sect;
	int         s;
	t_xy        v1;
	t_xy        v2;
	float       pcos;
	float       psin;
	float       tz1;
	float       tz2;
	float       tx1;
	float       tx2;
	float       nearz;
	float       farz;
	float       nearside;
	float       farside;
	t_xy        i1;
	t_xy        i2;
	t_xy        scale1;
	int         x1;
	t_xy        scale2;
	int         x2;
	float       yceil;
	float       yfloor;
	t_ab        y1;
	t_ab        y2;
	int         neighbor;
	float       nyceil;
	float       nyfloor;
	t_ab        ny1;
	t_ab        ny2;
	int         beginx;
	int         endx;
	int         xrw;
	t_ab        y;
	int         z;
	t_ab        cy;
	t_ab        ny;
	t_ab        cny;
	int         r1;
	int         r2;
	int         r;
}               t_dscreen;

typedef struct  s_square
{
    float 		square;
    float 		X;
    float 		Y;
    float 		X0;
    float 		Y0;
}               t_square;

typedef struct  s_line
{
    float       x0;
    float       y0;
    float       x1;
    float       y1;
    int			x;
    int			y;
    int         color;
	float		opacity;
}               t_line;

typedef struct	s_iline //init_line
{
	t_xy	zero;
	t_xy	one;
	int 	color;
}				t_iline;

/* Player: location */


typedef struct      s_xyz
{
    float           x;
    float           y;
    float           z;
}                   t_xyz;

typedef struct	s_pos
{
	int wsad[4];//={0,0,0,0};
	int ground;
	int falling;
	int moving;
	int ducking;
	int map;
}				t_pos;

/* Player: location */
typedef struct      s_player
{
    t_xyz           where;      /* Current position */
    t_xyz           velocity;   /* Current motion vector */
    float           angle;
    float 			anglesin;
    float 			anglecos;
    float 			yaw;		/* Looking towards (and sin() and cos() thereof) */
    unsigned char   num_sect;                   /* Which sector the player is currently in */
    t_pos			pos;

}                   t_player;

typedef struct      s_system
{
	unsigned		NumSectors;
	t_sector		*sectors;
	SDL_Surface		*surface;
	SDL_Window		*window;
	t_player		*player;
	t_dscreen       dscreen;
}                   t_system;

typedef struct      s_scaler
{
    int 			result;
    int 			bop;
    int 			fd;
	int 			ca;
	int				cache;

}                   t_scaler;

typedef struct      s_pixel
{
    float           r;
    float 		    g;
    float 		    b;
    float 		    brightness;
}                   t_pixel;

typedef struct      s_rgb
{
    int             r;
    int 		    g;
    int 		    b;
}                   t_rgb;

void        UnloadData(unsigned *NumSectors, t_sector *sectors);
void        LoadData_stable(t_player *player, t_system *system);
int         Scaler_Next(t_scaler* i);
void        plot(int x,int y, float opacity, int color, t_system *system);
void        BloomPostprocess(t_system *system);
void        fillpolygon(t_sector* sect, int color, t_system *system);
int         vert_compare(const t_xy* a, const t_xy* b);
void        vline(int x, int y1,int y2, int top,int middle,int bottom, t_system *system);
void        MovePlayer(float dx, float dy, t_player *player, t_sector *sectors);
void        SaveFrame2(t_system *system);
void        SaveFrame1(t_system *system);
void        VerifyMap(t_system *system, t_sector *sectors);
int         IntersectLineSegments(float x0,float y0, float x1,float y1,
                          float x2,float y2, float x3,float y3);
void        DrawScreen(t_player *player, t_system *system);
void        DrawMap(t_player *player, t_system *system, t_sector *sectors); // WTF?
int         done(t_system *system);
void	    ft_swap(float *x, float *y);
void	    line_l(t_iline *l, t_system *system);
void        line(float x0,float y0, float x1,float y1, int color, t_system *system);


int			key_event(SDL_Event *ev, t_player *player, t_system *system);
void		init_player(t_player *player);
void 		mouse_event(t_player *player, t_system *system, float *yaw);
void		move(t_player *player);
void		mvs(t_player *player, t_system *system, float const *eyeheight, int s);
void		mvs1(int s, t_sector *sector, float const *eyeh, t_system *system);
void		init_window(t_system *system);
void		falling(t_player *player, t_system *system, float const eyeheight);
int			main_cycle(t_player *player, t_system *system);
void		pos_m(t_system *system, t_player *player, float *eyeheight);

#endif
