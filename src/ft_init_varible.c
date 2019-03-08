/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_varible.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvoronyu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 19:54:10 by tvoronyu          #+#    #+#             */
/*   Updated: 2019/01/24 19:54:16 by tvoronyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void    createTables(t_wolf *wolf)
{
    wolf->fSinTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fISinTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fCosTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fICosTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fTanTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fITanTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fFishTable = newFloat(wolf->ANGLE60+1, ((void*)0));
    wolf->fXStepTable = newFloat(wolf->ANGLE360+1, ((void*)0));
    wolf->fYStepTable = newFloat(wolf->ANGLE360+1, ((void*)0));
}


void    writeTables(t_wolf *wolf)
{
    int i;
    float radian;

    i = -1;
    while (++i <= wolf->ANGLE360)
    {
        radian = arcToRad(i, wolf) + (float) (0.0001);

        wolf->fSinTable[i] = (float) sin((double) radian);
        wolf->fISinTable[i] = (1.0 / (wolf->fSinTable[i]));
        wolf->fCosTable[i] = (float) cos((double) radian);
        wolf->fICosTable[i] = (1.0 / (wolf->fCosTable[i]));
        wolf->fTanTable[i] = (float) tan((double) radian);
        wolf->fITanTable[i] = (1.0 / wolf->fTanTable[i]);

        if (i >= wolf->ANGLE90 && i < wolf->ANGLE270)
        {
            wolf->fXStepTable[i] = (float) (wolf->TILE_SIZE / wolf->fTanTable[i]);
            if (wolf->fXStepTable[i] > 0)
                wolf->fXStepTable[i] = -wolf->fXStepTable[i];
        }
        else
        {
            wolf->fXStepTable[i] = (float)(wolf->TILE_SIZE / wolf->fTanTable[i]);
            if (wolf->fXStepTable[i] < 0)
                wolf->fXStepTable[i] = -wolf->fXStepTable[i];
        }
        if (i >= wolf->ANGLE0 && i < wolf->ANGLE180)
        {
            wolf->fYStepTable[i] = (float)(wolf->TILE_SIZE * wolf->fTanTable[i]);
            if (wolf->fYStepTable[i] < 0)
                wolf->fYStepTable[i] = -wolf->fYStepTable[i];
        }
        else
        {
            wolf->fYStepTable[i] = (float)(wolf->TILE_SIZE * wolf->fTanTable[i]);
            if (wolf->fYStepTable[i] > 0)
                wolf->fYStepTable[i] = -wolf->fYStepTable[i];
        }
    }
    i = -wolf->ANGLE30;
    while (i <= wolf->ANGLE30)
    {
        radian = arcToRad(i, wolf);
        wolf->fFishTable[i + wolf->ANGLE30] = (float)(1.0 / cos((double)radian));
        i++;
    }
}



void    ft_view_float(float *f, int len)
{
    while (len--)
        printf("%f\n", f[len]);
}

void	ft_init_varible(t_wolf *wolf)
{
    wolf->loop = 1;
    wolf->loop2 = 1;
    wolf->TILE_SIZE = 260;
    wolf->WALL_HEIGHT = 260;
    wolf->PROJECTIONPLANEWIDTH = 1024;
    wolf->PROJECTIONPLANEHEIGHT = 768;
    wolf->ANGLE60 = wolf->PROJECTIONPLANEWIDTH;
    wolf->ANGLE30 = (wolf->ANGLE60/2);
    wolf->ANGLE15 = (wolf->ANGLE30/2);
    wolf->ANGLE90 = (wolf->ANGLE30*3);
    wolf->ANGLE180 = (wolf->ANGLE90*2);
    wolf->ANGLE270 = (wolf->ANGLE90*3);
    wolf->ANGLE360 = (wolf->ANGLE60*6);
    wolf->ANGLE0 = 0;
    wolf->ANGLE5 = (wolf->ANGLE30/6);
    wolf->ANGLE10 = (wolf->ANGLE5*2);
    wolf->fPlayerX = 460;
    wolf->fPlayerY = 610;
    wolf->fPlayerArc = wolf->ANGLE0;
    wolf->fPlayerDistanceToTheProjectionPlane = 1024 / 2 / (tan((3.14 / 180) * 30));
    wolf->fPlayerHeight = 130;
    wolf->fPlayerSpeed = 30;
    wolf->fProjectionPlaneYCenter = wolf->PROJECTIONPLANEHEIGHT/2;
    wolf->MAP_WIDTH = 12;
    wolf->MAP_HEIGHT = 12;
    wolf->sdl.wall[0] = load_image("./texture/11.jpg");
    wolf->sdl.floor = load_image("./texture/floor.jpg");
    wolf->sdl.up = load_image("./texture/ceil.jpg");
    wolf->checkWall = 0;
    wolf->raycast.plan_x = 0;
    wolf->raycast.plan_y = 66;
    wolf->speed = 0.25;
    wolf->koef = 0.05;
    wolf->tmpX = 0.0;
    wolf->tmpY = 0.0;
    wolf->ccc = 0;
    wolf->minDistanceToWall = 15;
//    printf("%f\n", wolf->raycast.plan_x);
//    printf("%f\n", wolf->raycast.plan_y);
}
