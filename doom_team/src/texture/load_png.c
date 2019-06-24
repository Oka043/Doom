#include "player_stable.h"


//void ld(t_system *system){
//	int k;
//
//	k = 0;
//	while (k<system->NumSectors)
//	{
//		system->sectors[k].floortexture = (void*) (texturedata + pos);
//		pos += sizeof(struct TextureSet);
//		system->sectors[k].ceiltexture  = (void*) (texturedata + pos);
//		pos += sizeof(struct TextureSet);
//		unsigned w;
//
//		w = system->sectors[k].npoints;
//		system->sectors[k].uppertextures = (void*) (texturedata + pos);
//		pos += sizeof(struct TextureSet) * w;
//		system->sectors[k].lowertextures = (void*) (texturedata + pos);
//		pos += sizeof(struct TextureSet) * w;
//		k++;
//	}
//}

//void 		loadImage(t_sprite *texture, int w, int h, char *str)
void	load_textures(t_system *system, char *string, Texture *tex)
{
	SDL_Surface		*image;
	unsigned int	*pixelss;


	image = IMG_Load(string);
	pixelss = (unsigned int*)image->pixels;
	if (!image) {
		printf("IMG_Load: %s\n", IMG_GetError());
		// handle error
	} else {
//		printf("IMG_Loaded '%s'\n", string);
//		printf("H = %d\nW = %d\n\n", image->h, image->w);
//		int i = -1;
//		int j;
//			while(++i < image->h)
//			{
//				j = -1;
//				while (++j < image->w)
//				{
//					*tex[i][j] = pixelss[i + (j * image->w)];
////					system->sectors->floortexture->texture[i][(j * image->w)] = pixelss[i + (j * image->w)];
//				}
//			}
//	}

		for (unsigned y = 0; y < TEXTURE_SIZE_Y; ++y)
			for (unsigned x = 0; x < TEXTURE_SIZE_X; ++x) {
				(*tex)[x][y] = image->format->Rmask + image->format->Gmask + image->format->Bmask;
			}
	}
}

//upload_items(main, 0, "media/sprites/items/medkit.png");
int load_items(t_system *system, char *string)
{
	SDL_Surface		*item;
	unsigned int	*pixels;
	int				x;
	int				y;

	item = IMG_Load(string);
	if (!item)
	{
		ft_putendl("Img_Load:");
		ft_putendl(IMG_GetError());
		return (-1);
	}
//	system->sprite = (int**)malloc(sizeof(int *) * 64);
	pixels = (unsigned int *)item->pixels;
	y = -1;
	while (++y < 64)
	{
		x = -1;
//		system->sprite[y] = (int *)malloc(sizeof(int) * 64);
		while (++x < 64)
			system->sprite.texture[y][x] = pixels[(y * item->w) + x];
	}
	SDL_FreeSurface(item);
	return 0;
}