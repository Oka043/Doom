//#include "player_stable.h"
//
////void    DrawMap(t_player *player, t_system *system, t_sector *sectors, t_line *lines)
//void    DrawMap(t_player *player, t_system *system, t_sector *sectors)
//{
//    static unsigned process = ~0u; ++process;
//    t_square s;
//    float x;
//    float y;
//
//
//    // Render the 2D map on screen
//    SDL_LockSurface(system->surface);
//#ifdef SplitScreen
//    for(unsigned y=0; y<H; ++y)
//        memset( (char*)system->surface->pixels + (y*W2 + W)*4, 0, (W2-W)*4);
//#else
//    for(unsigned y=0; y<H; ++y)
//        memset( (char*)system->surface->pixels + (y*W2)*4, 0, (W)*4);
//#endif
//#ifdef SplitScreen
//    s.square = min(W/20.f/0.8, H/29.f), s.X = (W2-W)/20.f/*square*0.8*/, s.Y = s.square, s.X0 = W+s.X*1.f/*(W-18*square*0.8)/2*/, s.Y0 = (H-28*s.square)/2;
//#else
//    s.square = min(W/20.f/0.8, H/29.f);//square
//#endif
//    s.X  = s.square*0.8;//X
//    s.Y = s.square;//Y
//    s.X0 = (W-18*s.square*0.8)/2;//X0
//    s.Y0 = (H-28*s.square)/2;//Y0
//
//    x = -1;
//    y = -1;
//    while (++x <= 18)
//		line_l(&(t_iline){ (t_xy){ s.X0+x*s.X, s.Y0+0*s.Y },
//						   (t_xy){ s.X0+ x*s.X, s.Y0+28*s.Y },
//						   0x002200 },system);
//    while (++y <= 28)
//		line_l(&(t_iline){ (t_xy){ s.X0+0*s.X, s.Y0+y*s.Y },
//						   (t_xy){ s.X0+18*s.X, s.Y0+ y*s.Y },
//						   0x002200 },system);
//
//#ifdef VisibilityTracking
//    for(unsigned c=0; c<system->NumSectors; ++c) if(sectors[c].visible) fillpolygon(&sectors[c], 0x220000, system);
//#endif
//    fillpolygon(&sectors[player->num_sect], 0x440000, system);
//
//#ifdef VisibilityTracking
//    for(unsigned c=0; c<NumVisibleSectors; ++c)
//    {
//        /*struct xy vert[W*2];
//        struct sector temp_sector = {0,0,vert,0,0,0};
//        for(unsigned x=0; x<W; ++x)  if(VisibleFloors[c][x]) vert[temp_sector.npoints++] = VisibleFloorBegins[c][x];
//        for(unsigned x=W; x-- > 0; ) if(VisibleFloors[c][x]) vert[temp_sector.npoints++] = VisibleFloorEnds[c][x];
//        fillpolygon(&temp_sector, 0x222200);
//        temp_sector.npoints = 0;
//        for(unsigned x=0; x<W; ++x)  if(VisibleCeils[c][x]) vert[temp_sector.npoints++] = VisibleCeilBegins[c][x];
//        for(unsigned x=W; x-- > 0; ) if(VisibleCeils[c][x]) vert[temp_sector.npoints++] = VisibleCeilEnds[c][x];
//        fillpolygon(&temp_sector, 0x220022);*/
//
//        for(unsigned x=0; x<W; ++x)
//        {
//            if(VisibleFloors[c][x])
//                line(clamp(s.X0 + VisibleFloorBegins[c][x].y*s.X, 0,W2-1), clamp(s.Y0 + (28-VisibleFloorBegins[c][x].x)*s.Y, 0,H-1),
//                     clamp(s.X0 + VisibleFloorEnds[c][x].y*s.X, 0,W2-1), clamp(s.Y0 + (28-VisibleFloorEnds[c][x].x)*s.Y, 0,H-1),
//                     0x222200, system);
//            if(VisibleCeils[c][x])
//                line(clamp(s.X0 + VisibleCeilBegins[c][x].y*s.X, 0,W2-1), clamp(s.Y0 + (28-VisibleCeilBegins[c][x].x)*s.Y, 0,H-1),
//                     clamp(s.X0 + VisibleCeilEnds[c][x].y*s.X, 0,W2-1), clamp(s.Y0 + (28-VisibleCeilEnds[c][x].x)*s.Y, 0,H-1),
//                     0x28003A, system);
//        }
//    }
//    /*for(unsigned n=0; n<NumVisibleFloors; ++n)
//    {
//        printf("%g,%g - %g,%g\n", VisibleFloorBegins[n].x, VisibleFloorBegins[n].y,
//                                  VisibleFloorEnds[n].x, VisibleFloorEnds[n].y );
//        line( X0+VisibleFloorBegins[n].x*X, Y0+VisibleFloorBegins[n].y*Y,
//              X0+VisibleFloorEnds[n].x*X,   Y0+VisibleFloorEnds[n].y*Y,
//              n*0x010101
//              //0x550055
//             );
//    }*/
//#endif
//
//    //for(unsigned c=0; c<system->NumSectors; ++c)
//    int i;
//
//    i = 0;
//    while (++i < system->NumSectors)
//	{
//		int a;
//
//		a = i;
//		if (a == player->num_sect && player->num_sect != (system->NumSectors-1))
//			a = system->NumSectors-1;
//		else if (a == system->NumSectors-1)
//			a = player->num_sect;
//
//		t_sector* sect;
//
//		sect = &sectors[a];
//		t_xy* vert;
//
//		vert = sect->vertex;
////		for(unsigned b = 0; b < sect->npoints; ++b)
//		int b;
//
//		b = 0;
//		while (++b < sect->npoints)
//		{
//			float x0;
//			float x1;
//			int vertcolor;
//
//			x0 = 28-vert[b].x;
//			x1 = 28-vert[b+1].x;
//			vertcolor = a==player->num_sect ? 0x55FF55 : 0x00AA00;
//
//			line_l(&(t_iline){ (t_xy){ s.X0+vert[b].y*s.X, s.Y0+x0*s.Y },
//							   (t_xy){ s.X0+vert[b+1].y*s.X, s.Y0+x1*s.Y },
//							   (a == player->num_sect)
//							   ? (sect->neighbours[b] >= 0 ? 0xFF5533 : 0xFFFFFF)
//							   : (sect->neighbours[b] >= 0 ? 0x880000 : 0x6A6A6A) },system);
//
//			line_l(&(t_iline){ (t_xy){ s.X0+vert[b].y*s.X-2, s.Y0+x0*s.Y-2 },
//							   (t_xy){ s.X0+vert[b].y*s.X+2, s.Y0+x0*s.Y-2 },
//							   vertcolor },system);
//
//			line_l(&(t_iline){ (t_xy){ s.X0+vert[b].y*s.X-2, s.Y0+x0*s.Y-2 },
//							   (t_xy){ s.X0+vert[b].y*s.X-2, s.Y0+x0*s.Y+2 },
//							   vertcolor },system);
//
//			line_l(&(t_iline){ (t_xy){ s.X0+vert[b].y*s.X+2, s.Y0+x0*s.Y-2 },
//							   (t_xy){ s.X0+vert[b].y*s.X+2, s.Y0+x0*s.Y+2 },
//							   vertcolor },system);
//
//			line_l(&(t_iline){ (t_xy){ s.X0+vert[b].y*s.X-2, s.Y0+x0*s.Y+2 },
//					  (t_xy){ s.X0+vert[b].y*s.X+2, s.Y0+x0*s.Y+2 },
//					  vertcolor },system);
//		}
//	}
//
//    float c;
//    float a;
//	t_xy	p;
//	t_xy	t;
//	t_xy	q_zero;
//	t_xy	q_one;
//
//	c = player->anglesin;
//	a = -player->anglecos;
//	p.x = player->where.y;
//	t.x = p.x + c*0.8f;
//	q_zero.x = p.x+a*0.2f;
//	q_one.x = p.x-a*0.2f;
//	p.y = 28 - player->where.x;
//	t.y = p.y +a*0.8f;
//	q_zero.y = p.y - c*0.2f;
//	q_one.y = p.y + c*0.2f;
//
//	p.x = clamp(p.x,-.4f,18.4f);
//	t.x = clamp(t.x,-.4f,18.4f);
//	q_zero.x = clamp(q_zero.x,-.4f,18.4f);
//	q_one.x = clamp(q_one.x,-.4f,18.4f);
//	p.y = clamp(p.y,-.4f,28.4f);
//	t.y = clamp(t.y,-.4f,28.4f);
//	q_zero.y = clamp(q_zero.y,-.4f,28.4f);
//	q_one.y = clamp(q_one.y,-.4f,28.4f);
//
//	line_l(&(t_iline){ (t_xy){ s.X0 + p.x*s.X, s.Y0 + p.y*s.Y },
//					   (t_xy){ s.X0 + t.x*s.X, s.Y0 + t.y*s.Y },
//					   0x5555FF },system);
//
//	line_l(&(t_iline){ (t_xy){ s.X0 +q_zero.x*s.X, s.Y0 +q_zero.y*s.Y },
//					   (t_xy){ s.X0 +q_one.x*s.X, s.Y0 +q_one.y*s.Y },
//					   0x5555FF },system);
//
//    BloomPostprocess(system);
//
//    SDL_UnlockSurface(system->surface);
//    SaveFrame1(system);
//
//    //static unsigned skip=0;
//    //if(++skip >= 1) { skip=0; SDL_Flip(system->surface); }
//}
