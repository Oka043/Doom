#ifndef DOOM_NUKEM_CAMERA_H
# define DOOM_NUKEM_CAMERA_H

#include "ft_math.h"
#include "window.h"

#define HFOV (0.73f*H)  // Affects the horizontal field of vision
#define VFOV (.2f*H)    // Affects the vertical field of vision

typedef struct	s_camera
{
	float 		nearz;
	float 		farz;
	float 		nearside;
	float 		farside;
	float 		hfov;
	float 		vfov;
}				t_camera;

void			clip_camera(t_camera *camera, t_vector *v1, t_vector *v2);
t_vector		line_to_screen(t_camera *camera,t_vector *v1, t_vector *v2);

#endif
