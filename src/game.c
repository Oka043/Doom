/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvoronyu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 19:25:50 by tvoronyu          #+#    #+#             */
/*   Updated: 2019/01/24 19:27:44 by tvoronyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

char **creatMapArray(int width, int height)
{
	char **map;

	map = NULL;
	map = (char**)malloc(sizeof(char *) * height + 1);
	map[height] = NULL;
	map[0]  = "WWWWWWWWWWWW";
	map[1]  = "WOOOOOOOOOOW";
	map[2]  = "WOOOOOOOOOOW";
	map[3]  = "WOOOOOOOOOOW";
	map[4]  = "WOOOOOOOOOOW";
	map[5]  = "WOOOOQOOOOOW";
	map[6]  = "WOOOOOOOOOOW";
	map[7]  = "WOOOOOOOOOOW";
	map[8]  = "WOOOOOOOOOOW";
	map[9]  = "WOOOOOOOOOOW";
	map[10] = "WOOOOOOOOOOW";
	map[11] = "WWWWWWWWWWWW";
	return (map);
}

void  creatMap(t_wolf *wolf)
{
	wolf->fMap = creatMapArray(wolf->MAP_WIDTH, wolf->MAP_HEIGHT);
}

float *newFloat(int len, float *tmp)
{
	tmp = (float*)malloc(sizeof(float) * len);
	ft_bzero(tmp, sizeof(float) * len);
	return (tmp);
}

float arcToRad(float arcAngle, t_wolf *wolf)
{
	return ((float)(arcAngle * 3.14159265359)/(float)wolf->ANGLE180);
}

void	render_texture(t_wolf *wolf)
{
	SDL_UpdateTexture(wolf->sdl.screen, NULL, wolf->buf, WIDTH << 2);
	SDL_RenderCopy(wolf->sdl.ren, wolf->sdl.screen, NULL, NULL);
	SDL_RenderCopy(wolf->sdl.ren,
				   wolf->sdl.font_text, NULL, &wolf->sdl.font_rect);
	SDL_RenderPresent(wolf->sdl.ren);
	SDL_DestroyTexture(wolf->sdl.font_text);
	SDL_FreeSurface(wolf->sdl.font_surface);
	TTF_CloseFont(wolf->sdl.font_ttf);
	free(wolf->test);
}

void    ft_render_3(t_wolf *wolf)
{
    if (wolf->distToHorizontalGridBeingHit < wolf->distToVerticalGridBeingHit)
    {
        wolf->dist = wolf->distToHorizontalGridBeingHit / wolf->fFishTable[wolf->castColumn];
        wolf->distortedDistance = wolf->dist;
        float ratio = wolf->fPlayerDistanceToTheProjectionPlane / wolf->dist;
        float scale;
        wolf->bottomOfWall = (ratio * wolf->fPlayerHeight + wolf->fProjectionPlaneYCenter);
        if (wolf->checkWall)
            scale = (wolf->fPlayerDistanceToTheProjectionPlane * (wolf->WALL_HEIGHT + wolf->ccc) / wolf->dist);
        else
            scale = (wolf->fPlayerDistanceToTheProjectionPlane * wolf->WALL_HEIGHT / wolf->dist);
        wolf->topOfWall = wolf->bottomOfWall - scale;
        wolf->xOffset = (int)wolf->xIntersection % wolf->TILE_SIZE;
    }
    else {
//			isVerticalHit = 1;
        wolf->dist = wolf->distToVerticalGridBeingHit / wolf->fFishTable[wolf->castColumn];
        float ratio = wolf->fPlayerDistanceToTheProjectionPlane / wolf->dist;
        float scale;
        wolf->bottomOfWall = (ratio * wolf->fPlayerHeight + wolf->fProjectionPlaneYCenter);
        if (wolf->checkWall) {
//            printf("fef");
            scale = (wolf->fPlayerDistanceToTheProjectionPlane * (wolf->WALL_HEIGHT + wolf->ccc) / wolf->dist);
        }
        else
            scale = (wolf->fPlayerDistanceToTheProjectionPlane * wolf->WALL_HEIGHT / wolf->dist);
        wolf->topOfWall = wolf->bottomOfWall - scale;
        wolf->xOffset = (int)wolf->yIntersection % wolf->TILE_SIZE;
    }
    wolf->checkWall = 0;
    drawWall(wolf->castColumn, wolf->topOfWall, 1, (wolf->bottomOfWall - wolf->topOfWall) + 1, wolf->xOffset, wolf);
    drawCeil(wolf->castColumn, wolf->topOfWall, wolf->bottomOfWall, wolf->bottomOfWall - wolf->topOfWall, wolf->xOffset, wolf);
    drawFloor(wolf->castColumn, wolf->topOfWall, wolf->bottomOfWall, wolf->bottomOfWall - wolf->topOfWall, wolf->xOffset, wolf);
    wolf->castArc++;
    wolf->checkWall = 0;
    if (wolf->castArc >= wolf->ANGLE360)
        wolf->castArc -= wolf->ANGLE360;
}

void    ft_render_2(t_wolf *wolf)
{
    if (wolf->castArc < wolf->ANGLE90 || wolf->castArc > wolf->ANGLE270)
    {
        wolf->verticalGrid = wolf->TILE_SIZE + (wolf->fPlayerX / wolf->TILE_SIZE) * wolf->TILE_SIZE;
        wolf->distToNextVerticalGrid = wolf->TILE_SIZE;

        float ytemp = wolf->fTanTable[wolf->castArc] * (wolf->verticalGrid - wolf->fPlayerX);
        wolf->yIntersection = ytemp + wolf->fPlayerY;
    }
    else
    {
        wolf->verticalGrid = (wolf->fPlayerX / wolf->TILE_SIZE) * wolf->TILE_SIZE;
        wolf->distToNextVerticalGrid = -wolf->TILE_SIZE;

        float ytemp = wolf->fTanTable[wolf->castArc] * (wolf->verticalGrid - wolf->fPlayerX);
        wolf->yIntersection = ytemp + wolf->fPlayerY;
        wolf->verticalGrid--;
    }
    if (wolf->castArc == wolf->ANGLE90 || wolf->castArc == wolf->ANGLE270)
        wolf->distToVerticalGridBeingHit = 9999;
    else
    {
        wolf->distToNextYIntersection = wolf->fYStepTable[wolf->castArc];
        while (1)
        {
            wolf->xGridIndex = (wolf->verticalGrid / wolf->TILE_SIZE);
            wolf->yGridIndex = (int)(wolf->yIntersection / wolf->TILE_SIZE);
            if ((wolf->xGridIndex >= wolf->MAP_WIDTH) ||
                (wolf->yGridIndex >= wolf->MAP_HEIGHT) ||
                wolf->xGridIndex < 0 || wolf->yGridIndex < 0)
            {
                wolf->distToVerticalGridBeingHit = 9999;
                break;
            }
            else if ((wolf->fMap[wolf->yGridIndex][wolf->xGridIndex]) != 'O')
            {
                if ((wolf->fMap[wolf->yGridIndex][wolf->xGridIndex]) == 'Q')
                    wolf->checkWall = 1;
                wolf->distToVerticalGridBeingHit =(wolf->yIntersection - wolf->fPlayerY) * wolf->fISinTable[wolf->castArc];
//                break;

                break;
            }
            else
            {
                wolf->yIntersection += wolf->distToNextYIntersection;
                wolf->verticalGrid += wolf->distToNextVerticalGrid;
            }
        }

    }
    ft_render_3(wolf);
//		float scaleFactor;
//		float dist;
//		int topOfWall;
//		int bottomOfWall;
//		int xOffset;
//		int isVerticalHit = 0;
//		int distortedDistance;

}

void    ft_render(t_wolf *wolf)
{
//	int 	verticalGrid;
//	int 	horizontalGrid;
//	int 	distToNextVerticalGrid;
//	int 	distToNextHorizontalGrid;
//	float 	xIntersection;
//	float 	yIntersection;
//	float 	distToNextXIntersection;
//	float 	distToNextYIntersection;
//	int 	xGridIndex;
//	int 	yGridIndex;
//	float 	distToVerticalGridBeingHit;
//	float 	distToHorizontalGridBeingHit;
//	int 	castColumn;

	wolf->castArc = wolf->fPlayerArc -wolf->ANGLE30;
    wolf->castColumn = -1;

	if (wolf->castArc < 0)
	{
		wolf->castArc = wolf->ANGLE360 + wolf->castArc;
	}
	while (++wolf->castColumn < wolf->PROJECTIONPLANEWIDTH)
	{
//	    if (wolf->checkWall)
//        {
//	        wolf->TILE_SIZE += 64;
//	        wolf->WALL_HEIGHT += 64;
//        }
//	    else
//        {
//            wolf->TILE_SIZE = 64;
//            wolf->WALL_HEIGHT = 64;
//        }
		if (wolf->castArc > wolf->ANGLE0 && wolf->castArc < wolf->ANGLE180)
		{
            wolf->horizontalGrid = (wolf->fPlayerY / wolf->TILE_SIZE) * wolf->TILE_SIZE + wolf->TILE_SIZE;
            wolf->distToNextHorizontalGrid = wolf->TILE_SIZE;
			float xtemp = wolf->fITanTable[wolf->castArc] * (wolf->horizontalGrid - wolf->fPlayerY);
            wolf->xIntersection = xtemp + wolf->fPlayerX;
//            ft_putnbr(wolf->horizontalGrid);
//            ft_putendl("");
		}
		else
		{
            wolf->horizontalGrid = (wolf->fPlayerY / wolf->TILE_SIZE) * wolf->TILE_SIZE;
            wolf->distToNextHorizontalGrid = -wolf->TILE_SIZE;
			float xtemp = wolf->fITanTable[wolf->castArc] * (wolf->horizontalGrid - wolf->fPlayerY);
            wolf->xIntersection = xtemp + wolf->fPlayerX;
            wolf->horizontalGrid--;
		}

		if (wolf->castArc == wolf->ANGLE0 || wolf->castArc == wolf->ANGLE180)
            wolf->distToHorizontalGridBeingHit = 9999;
		else
		{
            wolf->distToNextXIntersection = wolf->fXStepTable[wolf->castArc];
			while (1)
			{
                wolf->xGridIndex = (int)(wolf->xIntersection / wolf->TILE_SIZE);
                wolf->yGridIndex = (wolf->horizontalGrid / wolf->TILE_SIZE);

				if ((wolf->xGridIndex >= wolf->MAP_WIDTH) || (wolf->yGridIndex >= wolf->MAP_HEIGHT) || wolf->xGridIndex < 0 || wolf->yGridIndex < 0)
				{
                    wolf->distToHorizontalGridBeingHit = 9999;
					break;
				}
				else if ((wolf->fMap[wolf->yGridIndex][wolf->xGridIndex]) != 'O')
				{
                    if ((wolf->fMap[wolf->yGridIndex][wolf->xGridIndex]) == 'Q')
                        wolf->checkWall = 1;
                    wolf->distToHorizontalGridBeingHit  = (wolf->xIntersection - wolf->fPlayerX) * wolf->fICosTable[wolf->castArc];
//                    printf("%f\n", wolf->distToHorizontalGridBeingHit);


					break;
				}
				else
				{
                    wolf->xIntersection += wolf->distToNextXIntersection;
                    wolf->horizontalGrid += wolf->distToNextHorizontalGrid;
				}

			}

//            exit(0);
		}
        ft_render_2(wolf);

	}
}

//void	drawWall(int x, int yStart, int yEnd, int height, int pixels, t_wolf *wolf)
void	drawWall(int x, int y, int width, int height, int xOffset, t_wolf *wolf)
{
	int dy = height;
	x = floor(x);
	y = floor(y);

	if (x == wolf->PROJECTIONPLANEWIDTH)
        return ;
	xOffset = floor(xOffset);


	int sourceIndex = xOffset;

	int lastSourceIndex = sourceIndex + (wolf->TILE_SIZE * wolf->TILE_SIZE);

	int targetIndex = y;

	int heightToDraw = height;

	if (y + heightToDraw > HEIGHT)
		heightToDraw = HEIGHT - y;


	int yError=0;

	if (heightToDraw < 0)
		return ;

	int i = 0;
	while (1)
	{

		yError += height;


		i++;

		if (x < 0)
			x = 0;
		if (x > 1023)
			x = 1023;

		unsigned int *pixels;

		pixels = (unsigned int *)wolf->sdl.wall[0]->pixels;

        unsigned int color = pixels[sourceIndex];



		while (yError >= wolf->TILE_SIZE)
		{
			yError -= wolf->TILE_SIZE;

			if (targetIndex > 0 && targetIndex < 767)
			{
				wolf->buf[targetIndex][x] = color;
			}

			targetIndex++;

			heightToDraw--;
			if (heightToDraw < 1)
				return ;
		}
		sourceIndex += wolf->TILE_SIZE;
		if (sourceIndex > lastSourceIndex)
			sourceIndex = lastSourceIndex;
	}

//    if (yEnd >= HEIGHT)
//        yEnd = HEIGHT - 1;
//    if (yStart < 0)
//        yStart = 0;
//    if (yStart >= HEIGHT)
//        yStart = HEIGHT;
//    if (yEnd < 0)
//        yEnd = 0;
//	while (yStart <= yEnd)
//	{
//	    wolf->buf[yStart][x] = (256 * 256 * 255) + (256 * 255) + 255;
//	    yStart++;
//	}
}

//void	drawCeil(int x, int yStart, int yEnd, int height, int pixels, t_wolf *wolf)
void	drawCeil(int x, int y, int width, int height, int xOffset, t_wolf *wolf)
{
    int targetIndex = y;
    for (int row = y; row >= 0; row--)
    {
        float ratio = (wolf->WALL_HEIGHT - wolf->fPlayerHeight) / (wolf->fProjectionPlaneYCenter - row);

        int diagonalDistance = floor((wolf->fPlayerDistanceToTheProjectionPlane * ratio)*
                                        (wolf->fFishTable[x]));

        int yEnd = floor(diagonalDistance * wolf->fSinTable[wolf->castArc]);
        int xEnd = floor(diagonalDistance * wolf->fCosTable[wolf->castArc]);

        // Translate relative to viewer coordinates:
        xEnd += wolf->fPlayerX;
        yEnd += wolf->fPlayerY;

        // Get the tile intersected by ray:
        int cellX = floor(xEnd / wolf->TILE_SIZE);
        int cellY = floor(yEnd / wolf->TILE_SIZE);
        //console.log("cellX="+cellX+" cellY="+cellY);

        //Make sure the tile is within our map
        if ((cellX < wolf->MAP_WIDTH) && (cellY < wolf->MAP_HEIGHT) && cellX >= 0 && cellY >= 0)
        {
            // Find offset of tile and column in texture
            int tileRow = floor(yEnd % wolf->TILE_SIZE);
            int tileColumn = floor(xEnd % wolf->TILE_SIZE);
            // Pixel to draw
            if (targetIndex < HEIGHT && targetIndex >= 0)
            {
//                    unsigned int *pixels;
//
//                    pixels = (unsigned int *)wolf->sdl.wall[0]->pixels;

                unsigned int color = get_pixel_int(wolf->sdl.up, tileColumn, tileRow);

                wolf->buf[targetIndex][x] = color;
            }

            // Go to the next pixel (directly above the current pixel)
            targetIndex--;
        }
    }
}

//void	drawFloor(int x, int yStart, int yEnd, int height, int pixels, t_wolf *wolf)
void	drawFloor(int x, int y, int width, int height, int xOffset, t_wolf *wolf)
{


    int bytesPerPixel=4;
    int projectionPlaneCenterY = wolf->fProjectionPlaneYCenter;
    int lastBottomOfWall = floor(width);
    int lastTopOfWall = floor(y);

    //*************
    // FLOOR CASTING at the simplest!  Try to find ways to optimize this, you can do it!
    //*************
//    if (this.fFloorTextureBuffer!=undefined)
//    {
        // find the first bit so we can just add the width to get the
        // next row (of the same column)
        int targetIndex = lastBottomOfWall;
//        printf("%d\n", lastBottomOfWall);
        for (int row = lastBottomOfWall; row < wolf->PROJECTIONPLANEHEIGHT; row++)
        {
//printf("%d\n", lastBottomOfWall);
            float straightDistance = (wolf->fPlayerHeight) / (row - projectionPlaneCenterY) *
                    wolf->fPlayerDistanceToTheProjectionPlane;
//
            float actualDistance = straightDistance*
                               (wolf->fFishTable[x]);

//            printf("%f\n", actualDistance);
//
            int yEnd = floor(actualDistance * wolf->fSinTable[wolf->castArc]);
            int xEnd = floor(actualDistance * wolf->fCosTable[wolf->castArc]);

//            // Translate relative to viewer coordinates:
            xEnd += wolf->fPlayerX;
            yEnd += wolf->fPlayerY;
//
//
//            // Get the tile intersected by ray:
            int cellX = floor(xEnd / wolf->TILE_SIZE);
            int cellY = floor(yEnd / wolf->TILE_SIZE);
//            //console.log("cellX="+cellX+" cellY="+cellY);
//
//            //Make sure the tile is within our map
            if (cellX < wolf->MAP_WIDTH && cellY < wolf->MAP_HEIGHT && cellX >= 0 && cellY >= 0)
            {
//                // Find offset of tile and column in texture
                int tileRow = floor(yEnd % wolf->TILE_SIZE);
                int tileColumn = floor(xEnd % wolf->TILE_SIZE);
//                // Pixel to draw
//                int sourceIndex = ;

//                if (sourceIndex > 768432)
//                printf("%d\n", sourceIndex);
//                // Cheap shading trick
                if (targetIndex < HEIGHT && targetIndex >= 0)
                {
//                    unsigned int *pixels;
//
//                    pixels = (unsigned int *)wolf->sdl.wall[0]->pixels;

                    unsigned int color = get_pixel_int(wolf->sdl.floor, tileColumn, tileRow);

                    wolf->buf[targetIndex][x] = color;
                }

////                int brighnessLevel=(150/(actualDistance));
////                var red=Math.floor(this.fFloorTexturePixels[sourceIndex]*brighnessLevel);
////                var green=Math.floor(this.fFloorTexturePixels[sourceIndex+1]*brighnessLevel);
////                var blue=Math.floor(this.fFloorTexturePixels[sourceIndex+2]*brighnessLevel);
////                var alpha=Math.floor(this.fFloorTexturePixels[sourceIndex+3]);
//
////
//
//                // Draw the pixel
////                this.offscreenCanvasPixels.data[targetIndex]=red;
////                this.offscreenCanvasPixels.data[targetIndex+1]=green;
////                this.offscreenCanvasPixels.data[targetIndex+2]=blue;
////                this.offscreenCanvasPixels.data[targetIndex+3]=alpha;
////                wolf->buf[targetIndex][x] = color;
//                // Go to the next pixel (directly under the current pixel)
                targetIndex++;
            }
        }
//    }

//	int lastBottomOfWall = floor(width);
//	int lastTopOfWall = floor(y);
//
//	int targetIndex = lastBottomOfWall;
//
//	int row = lastBottomOfWall;
//
//	while (row++ < wolf->PROJECTIONPLANEHEIGHT)
//	{
//		float straightDistance = (wolf->fPlayerHeight) / (row - wolf->fProjectionPlaneYCenter) * wolf->fPlayerDistanceToTheProjectionPlane;
//		float actualDistance = straightDistance * (wolf->fFishTable[x]);
//
//		int yEnd = floor(actualDistance * wolf->fSinTable[wolf->castArc]);
//        int xEnd = floor(actualDistance * wolf->fCosTable[wolf->castArc]);
//
//        xEnd += wolf->fPlayerX;
//        yEnd += wolf->fPlayerY;
//
//        int cellX = floor(xEnd / wolf->TILE_SIZE);
//        int cellY = floor(yEnd / wolf->TILE_SIZE);
//
//        if ((cellX < wolf->MAP_WIDTH) && (cellY < wolf->MAP_HEIGHT) && cellX >= 0 && cellY >= 0)
//        {
//            int  tileRow = floor(yEnd % wolf->TILE_SIZE);
//            int  tileColumn = floor(xEnd % wolf->TILE_SIZE);
//            // Pixel to draw
//            int sourceIndex = (tileRow * wolf->TILE_SIZE) + tileColumn;
//
//            // Cheap shading trick
////            var brighnessLevel=(150/(actualDistance));
////            var red=Math.floor(this.fFloorTexturePixels[sourceIndex]*brighnessLevel);
////            var green=Math.floor(this.fFloorTexturePixels[sourceIndex+1]*brighnessLevel);
////            var blue=Math.floor(this.fFloorTexturePixels[sourceIndex+2]*brighnessLevel);
////            var alpha=Math.floor(this.fFloorTexturePixels[sourceIndex+3]);
//
//            unsigned int *pixels;
//
//            pixels = (unsigned int *)wolf->sdl.wall[0]->pixels;
//
//            unsigned int color = pixels[sourceIndex];
//
//            // Draw the pixel
////            this.offscreenCanvasPixels.data[targetIndex]=red;
////            this.offscreenCanvasPixels.data[targetIndex+1]=green;
////            this.offscreenCanvasPixels.data[targetIndex+2]=blue;
////            this.offscreenCanvasPixels.data[targetIndex+3]=alpha;
//
//            if (targetIndex > 0 && targetIndex < 767)
//            {
//                wolf->buf[targetIndex][x] = color;
//            }
//
//            // Go to the next pixel (directly under the current pixel)
//            targetIndex += wolf->TILE_SIZE;
//        }
//
////		row++;
//	}
//	if (yEnd >= HEIGHT)
//		yEnd = HEIGHT - 1;
//	if (yEnd < 0)
//	    yEnd = 0;
//	while (yEnd < HEIGHT - 1) {
//		wolf->buf[yEnd][x] = (256 * 256 * 20) + (256 * 255) + 255;
//		yEnd++;
//	}
}

void	ft_game(t_wolf *wolf)
{
	SDL_SetRelativeMouseMode(1);
	SDL_SetRelativeMouseMode(0);
	SDL_SetRelativeMouseMode(1);
//	SDL_WarpMouseInWindow(wolf->sdl.win, WIDTH / 2, HEIGHT / 2);
	Mix_PlayMusic(wolf->sdl.mus[0], -1);
	while (wolf->loop)
    {
        ft_fps(wolf);
        ft_init_ttf(wolf);
        SDL_WarpMouseInWindow(wolf->sdl.win, WIDTH / 2, HEIGHT / 2);
//        ft_threads(wolf);

        ft_render(wolf);
	    render_texture(wolf);
		keys(wolf);
        ft_jump(wolf);
	}
	SDL_DestroyTexture(wolf->sdl.screen);
	TTF_Quit();
}
