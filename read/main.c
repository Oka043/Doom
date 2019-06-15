#include "player_stable.h"

void SaveFrame1(t_system *system)
{
    return;
    char Buf[512];
    sprintf(Buf, "ffmpeg -an -f rawvideo -pix_fmt bgr0 -s %ux%u -r 60 -i - -aspect %u/%u -c:v h264 -crf 2 -preset fast -y file1.avi", W2,H, W2,H);

    static FILE* fp = NULL;
    if(!fp) {
        fp = /*fopen("file1.bin", "wb");*/ popen(Buf, "w");
    }
    fwrite(system->surface->pixels, W2*H, 4, fp);
    fflush(fp);
}

void SaveFrame2(t_system *system)
{
    return;
    //static unsigned skip=0;
    //if(++skip>=3) { skip=0; } else return;

    char Buf[512];
    sprintf(Buf, "ffmpeg -an -f rawvideo -pix_fmt bgr0 -s %ux%u -r 60 -i - -aspect %u/%u -c:v h264 -crf 2 -preset fast -y file2.avi", W2,H, W2,H);

    static FILE* fp = NULL;
    if(!fp) { fp = /*fopen("file2.bin", "wb");*/ popen(Buf, "w"); }
    fwrite(system->surface->pixels, W2*H, 4, fp);
    fflush(fp);
}

int IntersectLineSegments(float x0,float y0, float x1,float y1,
                                 float x2,float y2, float x3,float y3)
{
    return IntersectBox(x0,y0,x1,y1, x2,y2,x3,y3)
           && abs(PointSide(x2,y2, x0,y0,x1,y1) + PointSide(x3,y3, x0,y0,x1,y1)) != 2
           && abs(PointSide(x0,y0, x2,y2,x3,y3) + PointSide(x1,y1, x2,y2,x3,y3)) != 2;
}


// Scaler_Next: Return (b++ - a) * (f-d) / (c-a) + d using the initial values passed to Scaler_Init().

int Scaler_Next(t_scaler *i)
{
    for(i->cache += i->fd; i->cache >= i->ca; i->cache -= i->ca) i->result += i->bop;
    return i->result;
}

int vert_compare(const t_xy* a, const t_xy* b)
{
    if (a->y != b->y)
    	return (a->y - b->y) * 1e3;
    return (a->x - b->x) * 1e3;
}

void	init_window(t_system *system)
{
	SW = SDL_CreateWindow("Wolf3D",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, W, H, 0);
	system->surface = SDL_GetWindowSurface(SW);
}

int	main(int argc, char **argv)
{
    t_main  *main;
	// t_player	*player;
	// t_system	*system;
    main = init_main();
    sdl_init(main);
	// player = (t_player*)malloc(sizeof(t_player));
	// system = (t_system*)malloc(sizeof(t_system));
	// system->NumSectors = 0;
	// system->sectors = NULL;
	// LoadData_stable(player, system);
    
    init_all(main); //тут надо еще инициализировать небо
//	VerifyMap(system, system->sectors);
//	init_window(system);
//	SDL_ShowCursor(SDL_DISABLE);
//	init_player(player);
//	return (main_cycle(player, system));
}

t_main *init_main(void)
{
    t_main *main;

    if (!(main = ft_memalloc(sizeof(t_main))))
        print_error("Can't allocate memory");
    return (main);
}

void    sdl_init(t_main *main)
{
//	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);
	main->sdl.window = SDL_CreateWindow(
			"Doom",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			W,
			H,
			SDL_WINDOW_ALLOW_HIGHDPI);
	main->sdl.winsurface = SDL_GetWindowSurface(main->sdl.window);
	SDL_SetRelativeMouseMode(1);
	// тут еще можно вызывать фунцкию для подгрузки текстур
}