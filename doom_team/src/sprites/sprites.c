#include "player_stable.h"
#include "window.h"

#define true 1
#define false 0
#define bool int

#define numSprites 19

t_sprite sprite[numSprites] =
		{
				{20.5, 11.5, 10}, //green light in front of playerstart
				//green lights in every room
				{18.5,4.5, 10},
				{10.0,4.5, 10},
				{10.0,12.5,10},
				{3.5, 6.5, 10},
				{3.5, 20.5,10},
				{3.5, 14.5,10},
				{14.5,20.5,10},

				//row of pillars in front of wall: fisheye test
				{18.5, 10.5, 9},
				{18.5, 11.5, 9},
				{18.5, 12.5, 9},

				//some barrels around the map
				{21.5, 1.5, 8},
				{15.5, 1.5, 8},
				{16.0, 1.8, 8},
				{16.2, 1.2, 8},
				{3.5,  2.5, 8},
				{9.5,  15.5,8},
				{10.0, 15.1,8},
				{10.5, 15.8,8},
		};

//arrays used to sort the sprites
int spriteOrder[numSprites];
double spriteDistance[numSprites];

void combSort(int* order, double* dist, int amount)
{
	int gap = amount;
	bool swapped = false;
	while(gap > 1 || swapped)
	{
		//shrink factor 1.3
		gap = (gap * 10) / 13;
		if(gap == 9 || gap == 10) gap = 11;
		if (gap < 1) gap = 1;
		swapped = false;
		for (int i = 0; i < amount - gap; i++)
		{
			int j = i + gap;
			if (dist[i] < dist[j])
			{
				ft_swap_d(&dist[i], &dist[j]);
				ft_swap_d(&order[i], &order[j]);
				swapped = true;
			}
		}
	}
}

void	sprite_casting(t_system *system)
{

	double posX = system->player->where.x;
	double posY = system->player->where.y;
	double dirX = system->player->velocity.x;
	double dirY = system->player->velocity.y;
//	double planeX = system->player->
//	double planeY


//SPRITE CASTING
	//sort sprites from far to close
	for(int i = 0; i < numSprites; i++)
	{
		spriteOrder[i] = i;
		spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y)); //sqrt not taken, unneeded
	}
	combSort(spriteOrder, spriteDistance, numSprites);

	//after sorting the sprites, do the projection and draw them
	for(int i = 0; i < numSprites; i++)
	{
		//translate sprite position to relative to camera
		double spriteX = sprite[spriteOrder[i]].x - posX;
		double spriteY = sprite[spriteOrder[i]].y - posY;

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

		double transformX = invDet * (dirY * spriteX - dirX * spriteY);
		double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

		int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

		//parameters for scaling and moving the sprites
#define uDiv 1
#define vDiv 1
#define vMove 0.0
		int vMoveScreen = int(vMove / transformY);

		//calculate height of the sprite on screen
		int spriteHeight = abs(int(h / (transformY))) / vDiv; //using "transformY" instead of the real distance prevents fisheye
		//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
		if(drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
		if(drawEndY >= h) drawEndY = h - 1;

		//calculate width of the sprite
		int spriteWidth = abs( int (h / (transformY))) / uDiv;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if(drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if(drawEndX >= w) drawEndX = w - 1;

		//loop through every vertical stripe of the sprite on screen
		for(int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if(transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
				for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
				{
					int d = (y-vMoveScreen) * 256 - h * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
					int texY = ((d * texHeight) / spriteHeight) / 256;
					Uint32 color = texture[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
					if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
				}
		}
	}
}