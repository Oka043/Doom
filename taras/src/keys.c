/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvoronyu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 19:30:34 by tvoronyu          #+#    #+#             */
/*   Updated: 2019/01/24 19:39:13 by tvoronyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void    clearCanvas(t_wolf *wolf)
{
    int x, y;

    x = y = 0;

    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            wolf->buf[y][x] = 0;
            x++;
        }
        y++;
    }
}

void	ft_keys_2(t_wolf *wolf)
{
	if (wolf->keys[SDL_SCANCODE_LEFT])
	{

	}
	if (wolf->keys[SDL_SCANCODE_W])
	{
        wolf->dx = (int)(wolf->playerXDir * wolf->fPlayerSpeed);
        wolf->dy = (int)(wolf->playerYDir * wolf->fPlayerSpeed);
	}
}

void	ft_keys_3(t_wolf *wolf)
{
	if (wolf->keys[SDL_SCANCODE_S])
	{
        wolf->dx = -(int)(wolf->playerXDir * wolf->fPlayerSpeed);
        wolf->dy = -(int)(wolf->playerYDir * wolf->fPlayerSpeed);
	}
	if (wolf->keys[SDL_SCANCODE_UP] && wolf->fPlayerHeight + 1 < wolf->WALL_HEIGHT)
    {

    }
	if (wolf->keys[SDL_SCANCODE_DOWN] && wolf->fPlayerHeight - 10 > 0)
    {

    }
}

void	ft_keys_4(t_wolf *wolf)
{
	if (wolf->keys[SDL_SCANCODE_Q] && wolf->fPlayerHeight < wolf->WALL_HEIGHT - 20)
	{
		wolf->fPlayerHeight += 5;
	}
	if (wolf->keys[SDL_SCANCODE_E] && wolf->fPlayerHeight - 2 > 10)
	{
		wolf->fPlayerHeight -= 5;
	}

    if (wolf->keys[SDL_SCANCODE_D])
    {
        wolf->dx = (wolf->raycast.plan_x * wolf->speed);
        wolf->dy = (wolf->raycast.plan_y * wolf->speed);

        wolf->playerXCell = floor(wolf->fPlayerX / wolf->TILE_SIZE);
        wolf->playerYCell = floor(wolf->fPlayerY / wolf->TILE_SIZE);


        wolf->playerXCellOffset = wolf->fPlayerX % wolf->TILE_SIZE;
        wolf->playerYCellOffset = wolf->fPlayerY % wolf->TILE_SIZE;

//        printf("x%d\n", wolf->playerXCell);
//        printf("y%d\n", wolf->playerYCell);
//
//        printf("X%d\n", wolf->playerXCellOffset);
//        printf("Y%d\n", wolf->playerYCellOffset);

//        if (wolf->playerXCell < wolf->MAP_WIDTH - 2 && wolf->playerYCell < wolf->MAP_HEIGHT - 2 && wolf->playerXCell > 0 && wolf->playerYCell > 0 && )
//        {

//        if (wolf->playerYCell > 1 && wolf->playerXCell < (wolf->MAP_HEIGHT - 1))
//            wolf->fPlayerY += (int)wolf->dy;
//        else if (wolf->playerYCell == 1 && wolf->playerYCellOffset >= wolf->minDistanceToWall)
//            wolf->fPlayerY += (int)wolf->dy;
//        else if (wolf->playerYCell == wolf->MAP_HEIGHT - 2 && wolf->playerYCellOffset >= (wolf->TILE_SIZE - wolf->minDistanceToWall))
//            wolf->fPlayerX += (int)wolf->dx;
//        if (wolf->playerYCell > 1 && wolf->playerXCell <= (wolf->MAP_WIDTH - 1))
//            wolf->fPlayerX += (int)wolf->dx;
//        else if (wolf->playerXCell == 1 && wolf->playerXCellOffset > wolf->minDistanceToWall)
//            wolf->fPlayerX += (int)wolf->dx;
//        else if (wolf->playerXCell == wolf->MAP_WIDTH - 2 && wolf->playerXCellOffset >= (wolf->TILE_SIZE - wolf->minDistanceToWall))
//            wolf->fPlayerX += (int)wolf->dx;
            wolf->fPlayerX += (int)wolf->dx;
            wolf->fPlayerY += (int)wolf->dy;
//        }
//        printf("DX = %f\n", wolf->dx);
//        printf("DY = %f\n", wolf->dy);
    }
    if (wolf->keys[SDL_SCANCODE_A])
    {
        wolf->dx = (wolf->raycast.plan_x * wolf->speed);
        wolf->dy = (wolf->raycast.plan_y * wolf->speed);

        wolf->playerXCell = floor(wolf->fPlayerX / wolf->TILE_SIZE);
        wolf->playerYCell = floor(wolf->fPlayerY / wolf->TILE_SIZE);

        wolf->playerXCellOffset = wolf->fPlayerX % wolf->TILE_SIZE;
        wolf->playerYCellOffset = wolf->fPlayerY % wolf->TILE_SIZE;

//        printf("x%d\n", wolf->playerXCell);
//        printf("y%d\n", wolf->playerYCell);
//
//        printf("X%d\n", wolf->playerXCellOffset);
//        printf("Y%d\n", wolf->playerYCellOffset);

//        if (wolf->playerYCell > 1 && wolf->playerXCell < (wolf->MAP_HEIGHT - 1))
            wolf->fPlayerY -= (int)wolf->dy;
//        else if (wolf->playerYCell == 1 && wolf->playerYCellOffset - 3 > wolf->minDistanceToWall)
//            wolf->fPlayerY -= (int)wolf->dy;
//        else if (wolf->playerYCell == wolf->MAP_HEIGHT - 2 && wolf->playerYCellOffset - 5 > (wolf->TILE_SIZE - wolf->minDistanceToWall))
//            wolf->fPlayerX -= (int)wolf->dx;
//        if (wolf->playerYCell > 1 && wolf->playerXCell < (wolf->MAP_WIDTH - 1))
//            wolf->fPlayerX -= (int)wolf->dx;
//        else if (wolf->playerXCell == 1 && wolf->playerXCellOffset - 5 > wolf->minDistanceToWall)
//            wolf->fPlayerX -= (int)wolf->dx;
//        else if (wolf->playerXCell == wolf->MAP_WIDTH - 2 && wolf->playerXCellOffset - 5 > (wolf->TILE_SIZE - wolf->minDistanceToWall))
            wolf->fPlayerX -= (int)wolf->dx;

//        printf("AX = %f\n", wolf->dx);
//        printf("AY = %f\n", wolf->dy);
//        printf("Y = %d\n", wolf->fPlayerY);
//        printf("X = %d\n", wolf->fPlayerX);
    }
}

void	ft_keys_help(t_wolf *wolf)
{
	ft_keys_1(wolf);
	ft_keys_2(wolf);
	ft_keys_3(wolf);
//	ft_forward_back(wolf);
    wolf->fPlayerX += wolf->dx;
    wolf->fPlayerY += wolf->dy;
	ft_keys_4(wolf);
//    ft_check_left_right(wolf);


}

void    ft_forward_back(t_wolf *wolf)
{
    wolf->fPlayerX += wolf->dx;
    wolf->fPlayerY += wolf->dy;

    wolf->playerXCell = floor(wolf->fPlayerX / wolf->TILE_SIZE);
    wolf->playerYCell = floor(wolf->fPlayerY / wolf->TILE_SIZE);

    wolf->playerXCellOffset = wolf->fPlayerX % wolf->TILE_SIZE;
    wolf->playerYCellOffset = wolf->fPlayerY % wolf->TILE_SIZE;


    if (wolf->dx > 0)
    {
        // moving right
        if ((wolf->fMap[wolf->playerYCell][wolf->playerXCell + 1] != 'O') && (wolf->playerXCellOffset > (wolf->TILE_SIZE - wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerX -= (wolf->playerXCellOffset - (wolf->TILE_SIZE - wolf->minDistanceToWall));
        }
    }
    else
    {
        // moving left
        if ((wolf->fMap[wolf->playerYCell][wolf->playerXCell - 1] != 'O') && (wolf->playerXCellOffset < (wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerX += (wolf->minDistanceToWall - wolf->playerXCellOffset);
        }
    }

    if (wolf->dy > 0)
    {
        // moving up
        if ((wolf->fMap[wolf->playerYCell-1][wolf->playerXCell] != 'O') && (wolf->playerYCellOffset < (wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerY += (wolf->minDistanceToWall - wolf->playerYCellOffset);
        }
    }
    else
    {
        // moving down
        if ((wolf->fMap[wolf->playerYCell+1][wolf->playerXCell] != 'O') && (wolf->playerYCellOffset > (wolf->TILE_SIZE - wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerY -= (wolf->playerYCellOffset - (wolf->TILE_SIZE - wolf->minDistanceToWall ));
        }
    }
}

void    ft_check_left_right(t_wolf *wolf)
{
    wolf->playerXCell = floor(wolf->fPlayerX / wolf->TILE_SIZE);
    wolf->playerYCell = floor(wolf->fPlayerY / wolf->TILE_SIZE);

    wolf->playerXCellOffset = wolf->fPlayerX % wolf->TILE_SIZE;
    wolf->playerYCellOffset = wolf->fPlayerY % wolf->TILE_SIZE;

//    printf("%d\n", wolf->playerXCell);
//    printf("%d\n", wolf->playerYCell);
//    printf("%d\n", wolf->playerXCellOffset);
//    printf("%d\n", wolf->playerYCellOffset);


    if (wolf->dx > 0)
    {
        // moving right
        if ((wolf->fMap[wolf->playerYCell][wolf->playerXCell + 1] != 'O') && (wolf->playerXCellOffset > (wolf->TILE_SIZE - wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerX -= (wolf->playerXCellOffset - (wolf->TILE_SIZE - wolf->minDistanceToWall));
        }
    }
    else
    {
        // moving left
        if ((wolf->fMap[wolf->playerYCell][wolf->playerXCell - 1] != 'O') && (wolf->playerXCellOffset < (wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerX += (wolf->minDistanceToWall - wolf->playerXCellOffset);
        }
    }

    if (wolf->dy < 0)
    {
        // moving up
        if ((wolf->fMap[wolf->playerYCell-1][wolf->playerXCell] != 'O') && (wolf->playerYCellOffset < (wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerY += (wolf->minDistanceToWall - wolf->playerYCellOffset);
        }
    }
    else
    {
        // moving down
        if ((wolf->fMap[wolf->playerYCell+1][wolf->playerXCell] != 'O') && (wolf->playerYCellOffset > (wolf->TILE_SIZE - wolf->minDistanceToWall)))
        {
            // back player up
            wolf->fPlayerY -= (wolf->playerYCellOffset - (wolf->TILE_SIZE - wolf->minDistanceToWall ));
        }
    }
}

void	ft_keys_1(t_wolf *wolf)
{
    if (wolf->keys[SDL_SCANCODE_LSHIFT] == 1)
        wolf->fPlayerSpeed = 60;
    else
    {
        if (wolf->keys[SDL_SCANCODE_LALT] == 1)
            wolf->fPlayerSpeed = 15;
        else
            wolf->fPlayerSpeed = 30;
    }

    if (wolf->keys[SDL_SCANCODE_J] == 1)
        wolf->ccc += 5;

    if (wolf->keys[SDL_SCANCODE_K] == 1)
        wolf->ccc -= 5;
}

void	ft_moveLeft(t_wolf *wolf)
{
	if (wolf->sdl.event.motion.xrel > 0 && wolf->sdl.event.motion.xrel < 1000)
	{
        wolf->plan = wolf->raycast.plan_x;
        wolf->raycast.plan_x = wolf->raycast.plan_x *
                               cos((fabs(wolf->sdl.event.motion.xrel / 700.0))) -
                               wolf->raycast.plan_y *
                               sin((fabs(wolf->sdl.event.motion.xrel / 700.0)));
        wolf->raycast.plan_y = wolf->plan *
                               sin((fabs(wolf->sdl.event.motion.xrel / 700.0))) +
                               wolf->raycast.plan_y *
                               cos((fabs(wolf->sdl.event.motion.xrel / 700.0)));
		if ((wolf->fPlayerArc += (wolf->sdl.event.motion.xrel * 1.5)) >= wolf->ANGLE360)
			wolf->fPlayerArc -= wolf->ANGLE360;
		wolf->sdl.event.motion.xrel = 0;
	}
	else if (wolf->sdl.event.motion.xrel > -1000 && wolf->sdl.event.motion.xrel < 0)
	{
        wolf->plan = wolf->raycast.plan_x;
        wolf->raycast.plan_x = wolf->raycast.plan_x *
                               cos(-(fabs(wolf->sdl.event.motion.xrel / 700.0))) -
                               wolf->raycast.plan_y *
                               sin(-(fabs(wolf->sdl.event.motion.xrel / 700.0)));
        wolf->raycast.plan_y = wolf->plan *
                               sin(-(fabs(wolf->sdl.event.motion.xrel / 700.0))) +
                               wolf->raycast.plan_y *
                               cos(-(fabs(wolf->sdl.event.motion.xrel / 700.0)));
		if ((wolf->fPlayerArc -= -(wolf->sdl.event.motion.xrel * 1.5)) <= wolf->ANGLE0)
			wolf->fPlayerArc += wolf->ANGLE360;
		wolf->sdl.event.motion.xrel = 0;
	}
	if (wolf->sdl.event.motion.yrel > 0 && wolf->sdl.event.motion.yrel < 1000)
	{
		wolf->fProjectionPlaneYCenter -= wolf->sdl.event.motion.yrel;
		wolf->sdl.event.motion.yrel = 0;
	}
	else if (wolf->sdl.event.motion.yrel > -1000 && wolf->sdl.event.motion.yrel < 0)
	{
	    wolf->fProjectionPlaneYCenter += -wolf->sdl.event.motion.yrel;
		wolf->sdl.event.motion.yrel = 0;
	}
//	printf("plan_x = %f\n", wolf->raycast.plan_x);
//	printf("plan_y = %f\n", wolf->raycast.plan_y);
}

void	keys(t_wolf *wolf)
{
    wolf->dx = wolf->dy = 0;
	wolf->playerXDir = wolf->fCosTable[wolf->fPlayerArc];
	wolf->playerYDir = wolf->fSinTable[wolf->fPlayerArc];

    clearCanvas(wolf);

	wolf->keys = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&wolf->sdl.event))
		ft_moveLeft(wolf);
    ft_pause_2(wolf);
	ft_keys_help(wolf);
}
