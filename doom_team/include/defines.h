#ifndef DOOM_NUKEM_DEFINES_H
#define DOOM_NUKEM_DEFINES_H


/* Define camera height from floor and distance to ceiling */
#define EyeHeight  6 /* Camera height from floor when standing */
#define DuckHeight 2.5 /* And when crouching */
#define HeadMargin 1 /* How much room there is above camera before the head hits the ceiling */
#define KneeHeight 2 /* How tall obstacles the player can simply walk over without jumping */

/* Define factors that affect the field of vision (never >= 180 degrees though) */

#define hfov (1.0 * 0.73f*H/W) /* Affects the horizontal field of vision */
#define vfov (1.0 * .2f) /* Affects the vertical field of vision */

#define Yaw(y,z) (y + z*player->yaw)

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

#define Scaler_Init(a,b,c,d,f) \
    { d + (b-1 - a) * (f-d) / (c-a), ((f<d) ^ (c<a)) ? -1 : 1, \
      abs(f-d), abs(c-a), (int)((b-1-a) * abs(f-d)) % abs(c-a) }

// Helper function for the antialiased line algorithm.
#define fpart(x) ((x) < 0 ? 1 - ((x) - floorf(x)) : (x) - floorf(x))
#define rfpart(x) (1 - fpart(x))

// Some hard-coded limits.
#define MAX_VERTICLES 100 // maximum number of vertices in a map
#define MAX_EDGES 100    // maximum number of edges in a sector
#define MAX_QUEUE 32     // maximum number of pending portal renders

#define TEXTURE_SIZE_X 64
#define TEXTURE_SIZE_Y 64


#define ABS(X)		(((X) < 0) ? (-(X)) : (X))
#define M1(X)		((X) - 1)

/*
**
**   CUSTOM DEFINES
**
*/

#define PP player->pos
#define KEY ev->key.keysym.sym
#define SQ(a) (a*a)
#define PW player->where
#define PV player->velocity
#define SS system->sectors
#define SEV sector->vertex
#define SW system->window
#define SDSC system->dscreen


#endif //DOOM_NUKEM_DEFINES_H
