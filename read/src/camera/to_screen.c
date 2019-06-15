#include "camera.h"

t_vector	line_to_screen(t_camera *camera, t_vector *start, t_vector *end)
{
	/* If it's partially behind the player, clip it against player's view frustrum */
	if(start->y <= 0 || end->y <= 0)
	{
		float nearz = 1e-4f, farz = 5.f, nearside = 1e-5f, farside = 20.f;
		// Find an intersection between the wall and the approximate edges of player's view
		t_vector i1 = Intersect(start->x,start->y,end->x,end->y, -nearside,nearz, -farside,farz);
		t_vector i2 = Intersect(start->x,start->y,end->x,end->y,  nearside,nearz,  farside,farz);
		if(start->y < nearz) { if(i1.y > 0) { start->x = i1.x; start->y = i1.y; } else { start->x = i2.x; start->y = i2.y; } }
		if(end->y < nearz) { if(i1.y > 0) { end->x = i1.x; end->y = i1.y; } else { end->x = i2.x; end->y = i2.y; } }
	}
	float xscale1 = camera->hfov / start->y;
	int x1 = W/2 - (int)(start->x * xscale1);
	float xscale2 = camera->hfov / end->y;
	int x2 = W/2 - (int)(end->x * xscale2);

	return ((t_vector){x1, x2});
}