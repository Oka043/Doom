#include "player_stable.h"

void save_frame1(t_system *system)
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

void save_frame2(t_system *system)
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

int	main(int argc, char **argv)
{
	//{
	//	 t_player	*player;
	//	 t_system	*system;
	//
	//	(void) argc;
	//	(void) argv;
	//	 player = (t_player*)malloc(sizeof(t_player));
	//	 system = (t_system*)malloc(sizeof(t_system));
	//	 system->NumSectors = 0;
	//	 system->sectors = NULL;
	//
	//    system = init_system();
	//    sdl_init(system);
	//
	//
	//
	//
	//	LoadData_stable(player, system);
	//	system->player = player;
	////    read_file(player, system);
	////    player = system->player;
	////	verify_map(system, system->sectors);
	//	SDL_ShowCursor(SDL_ENABLE);
	//	init_player(system->player);
	//	return (main_cycle(system->player, system));
	//}

	t_player *player;
	t_system *system;

	player = (t_player *)malloc(sizeof(t_player));
	system = (t_system *)malloc(sizeof(t_system));

	sdl_init(system);

	system->wh.w       = W;
	system->wh.h       = H;
	system->NumSectors = 0;
	system->sectors    = NULL;

	system->player = player;
	read_file(system);
	player = system->player;


	int textures_initialized = LoadTexture(system);
//	system = init_system();

	//		Texture *FloorTexture;
	//		FloorTexture = 0;

	//	loadImage(texture[8], tw, th, "pics/barrel.png");
	//	load_textures(system, "assets/bricksx64.png");
	//	load_items(system, "assets/kirito.png");


	SDL_ShowCursor(SDL_DISABLE);
	init_player(player);
	system->player = NULL;
	system->vertex = NULL;
	system->sectors->transit_array = NULL;
	return (main_cycle(player, system));
}

t_system *init_system(void)
{
    t_system *system;

    if (!(system = ft_memalloc(sizeof(t_system))))
        print_error("Can't allocate memory");
    return (system);
}

void    sdl_init(t_system *system)
{
//	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);
	system->window = SDL_CreateWindow(
			"Doom",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			W,
			H,
			SDL_WINDOW_ALLOW_HIGHDPI);
	system->surface = SDL_GetWindowSurface(system->window);
	SDL_SetRelativeMouseMode(1);
	// тут еще можно вызывать фунцкию для подгрузки текстур
}