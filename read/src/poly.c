#include "player_stable.h"

///* fillpolygon draws a filled polygon -- used only in the 2D map rendering. */
//void	fillpolygon(t_sector* sect, int color, t_system *system)
//{
//	t_line	l;
//    float	square;
//    t_xy*	vert;
//    float	miny;
//    float	maxy;
//
//    unsigned itr;
//
//    itr = 0;
//
//#ifdef SplitScreen
//    square = min(W/20.f/0.8, H/29.f);
//    l.x = square*0.8;
//    l.y = square;
//    l.x0 = (W-18*square*0.8)/2;
//    l.y0 = (H-28*square)/2;

//
//
////    square = min(W/20.f/0.8, H/29.f), l.x = (W2-W)/20.f/*square*0.8*/, l.y = square, l.x0 = W+l.x*1.f/*(W-18*square*0.8)/2*/, l.y0 = (H-28*square)/2;
//#else
//    square = min(W/20.f/0.8, H/29.f), l.x = square*0.8, l.y = square, l.x0 = (W-18*square*0.8)/2, l.y0 = (H-28*square)/2;
//#endif
//
//    vert = sect->vertex;
//    // Find the minimum and maximum Y coordinates
//    miny = 9e9;
//    maxy = -9e9;
//
//    //    while(++itr < sect->npoints)
//
//    for(unsigned a = 0; a < sect->npoints; ++a)
//    {
//        printf("a = %d \n", a);
//        miny = min(miny, 28-vert[a].x);
//        maxy = max(maxy, 28-vert[a].x);
//    }
//    miny = l.y0 + miny*l.y; maxy = l.y0 + maxy*l.y;
//    // Scan each line within this range
//    for(int y = max(0, (int)(miny+0.5)); y <= min(H-1, (int)(maxy+0.5)); ++y)
//    {
//        // Find all intersection points on this scanline
//        float intersections[W2];
//        unsigned num_intersections = 0;
//        for(unsigned a = 0; a < sect->npoints && num_intersections < W; ++a)
//        {
//            float x0 = l.x0+vert[a].y*l.x, x1 = l.x0+vert[a+1].y*l.x;
//            float y0 = l.y0+(28-vert[a].x)*l.y, y1 = l.y0+(28-vert[a+1].x)*l.y;
//            if(IntersectBox(x0,y0,x1,y1, 0,y,W2-1,y))
//            {
//                t_xy point = INTERSECT(x0,y0,x1,y1, 0,y,W2-1,y);
//                if(isnan(point.x) || isnan(point.y))
//                    continue;
//                // Insert it in intersections[] keeping it sorted.
//                // Sorting complexity: n log n
//                unsigned begin = 0, end = num_intersections, len = end-begin;
//                while(len)
//                {
//                    unsigned middle = begin + len/2;
//                    if(intersections[middle] < point.x)
//                    { begin = middle++; len = len - len/2 - 1; }
//                    else
//                        len /= 2;
//                }
//                for(unsigned n = num_intersections++; n > begin; --n)
//                    intersections[n] = intersections[n-1];
//                intersections[begin] = point.x;
//            }
//        }
//        // Draw lines
//        for(unsigned a = 0; a+1 < num_intersections; a += 2)
//            line(clamp(intersections[a],  0,W2-1), y, clamp(intersections[a+1],0,W2-1), y, color, system);
//        //printf("line(%f,%d, %f,%d)\n", minx,y, maxx,y);
//    }
//}

void fillpolygon(t_sector* sect, int color, t_system *system)
{

	float square;
	float X;
	float Y;
	float X0;
	float Y0;
	t_xy* vert;
	float miny;
	float maxy;

	unsigned itr;

	itr = 0;
	vert = sect->vertex;
	// Find the minimum and maximum Y coordinates
	miny = 9e9;
	maxy = -9e9;

	//    while(++itr < sect->npoints)

	int a;

	a = -1;
	while (++a < sect->npoints)
	{
		printf("a = %d \n", a);
		miny = min(miny, 28-vert[a].x);
		maxy = max(maxy, 28-vert[a].x);
	}
	miny = Y0 + miny*Y; maxy = Y0 + maxy*Y;
	// Scan each line within this range
	int y;

	y = max(0, (int)(miny+0.5));
	while (y <= min(H-1, (int)(maxy+0.5)))
	{
		// Find all intersection points on this scanline
		float intersections[W2];
		int num_intersections = 0;

		int b;

		b = -1;
		while (++b < sect->npoints && num_intersections < W)
		{
			float x0;
			float x1;

			x0 = X0+vert[a].y*X;
			x1 = X0+vert[a+1].y*X;

			float y0;
			float y1;

			y0 = Y0+(28-vert[a].x)*Y;
			y1 = Y0+(28-vert[a+1].x)*Y;

			if(IntersectBox(x0,y0,x1,y1, 0,y,W2-1,y))
			{
				t_xy point;

				point = Intersect(x0,y0,x1,y1, 0,y,W2-1,y);
				if(isnan(point.x) || isnan(point.y))
					continue;
				// Insert it in intersections[] keeping it sorted.
				// Sorting complexity: n log n
				int begin;
				int end;
				int len;

				begin = 0;
				end = num_intersections;
				len = end-begin;
				while(len)
				{
					unsigned middle;
					middle = begin + len/2;
					if(intersections[middle] < point.x)
					{
						begin = middle++;
						len = len - len/2 - 1;
					}
					else
						len /= 2;
				}
				int n;

				n = num_intersections++;
				while (n > begin)
				{
					intersections[n] = intersections[n-1];
					n--;
				}
				intersections[begin] = point.x;
			}
		}
		// Draw lines
		int a;

		a = 0;
		while (a+1 < num_intersections)
		{
			line_l(&(t_iline){ (t_xy){ clamp(intersections[a], 0,W2-1), y },
			                   (t_xy){ clamp(intersections[a+1], 0,W2-1), y },
			                   color },system);
			a += 2;
		}
		y++;
		//printf("line(%f,%d, %f,%d)\n", minx,y, maxx,y);
	}
}

