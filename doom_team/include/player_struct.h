#ifndef DOOM_NUKEM_PLAYER_STRUCT_H
#define DOOM_NUKEM_PLAYER_STRUCT_H

#include "player_stable.h"
#include "defines.h"
#include "window.h"

typedef int Texture[64][64];

typedef struct	s_texture {
	int 	texture[64][64];
}				t_texture;
struct TextureSet { Texture texture, normalmap, lightmap, lightmap_diffuseonly; };

struct Scaler { int result, bop, fd, ca, cache; };

typedef struct 		s_vertex
{
	double 			*x_arr;
	size_t			x_arr_size;
	float			x;
	float			y;
	float			z;

}					t_vertex;

typedef struct 	    s_xy
{
	float 		    x;
	float 		    y;
//	float 			m; //x
//	float			n; //y
}                   t_xy;

typedef struct      s_ab
{
	int             a;
	int             b;
}                   t_ab;

typedef struct		s_wall
{
	int32_t 		is_wall;
	float 			start_wall[2];
	float 			end_wall[2];
}					t_wall;

typedef struct      s_sector
{
	float           floor;
	float           ceil;
	t_xy            *vertex;    /* Each vertex has an x and y coordinate */
//	int             npoints;    /* How many vertexes there are */
	int32_t     	*neighbours; /* Each pair of vertexes may have a corresponding neighbouring sector */
	struct TextureSet *floortexture, *ceiltexture, *uppertextures, *lowertextures;
	t_texture		textures;

	t_wall			*transit_array;
	size_t          npoints;    /* How many vertexes there are */ //verr_arr_len

	//
	size_t			sec_index; // номер сектора

	size_t			ver_arr_len; // the same as npoints
	size_t			color;
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
	t_xy        scale2;

	int         x1;
	int         x2;

	float       yceil;
	float       yfloor;

	t_ab        y1;
	t_ab        y2;

	int         neighbour;
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
//	unsigned char   num_sect;                   /* Which sector the player is currently in */
	t_pos			pos;
	int32_t			num_sect;

}                   t_player;

typedef struct	s_sprite
{
	double x;
	double y;
	int			texture[TEXTURE_SIZE_X][TEXTURE_SIZE_Y];
}				t_sprite;

typedef struct      s_system
{
	unsigned		NumSectors;
	t_sector		*sectors;
	SDL_Surface		*surface;
	SDL_Window		*window;
	t_player		*player;
	t_dscreen       dscreen;
	t_window		wh;
	t_sprite		sprite;
	//
	t_vertex		*vertex; // массив структур всех точек карты
	size_t			sum_vert;
	size_t			sum_vert_pair;

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







#endif //DOOM_NUKEM_PLAYER_STRUCT_H
