/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <osak@student.unit.ua>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 20:06:43 by osak              #+#    #+#             */
/*   Updated: 2019/06/10 20:06:44 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player_stable.h"

int		key_event(SDL_Event *ev, t_player *player, t_system *system)
{
	KEY == 'w' ? PP.wsad[0] = ev->type == SDL_KEYDOWN : 0;
	KEY == 's' ? PP.wsad[1] = ev->type == SDL_KEYDOWN : 0;
	KEY == 'a' ? PP.wsad[2] = ev->type == SDL_KEYDOWN : 0;
	KEY == 'd' ? PP.wsad[3] = ev->type == SDL_KEYDOWN : 0;
	if (KEY == 'q' || KEY == SDLK_ESCAPE)
		return (done(system));
	if (KEY == ' ')
	{
		if (PP.ground)
		{
			player->velocity.z += 0.5;
			PP.falling = 1;
		}
	}
	if (KEY == SDLK_LCTRL || KEY == SDLK_RCTRL)
	{
		PP.ducking = ev->type == SDL_KEYDOWN;
		PP.falling = 1;
	}
	if (KEY == SDL_QUIT)
		return (done(system));
	return (1);
}

void	mouse_event(t_player *player, t_system *system, float *yaw)
{
	int		x;
	int		y;

	SDL_GetRelativeMouseState(&x, &y);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	player->angle += x * 0.03f;
	*yaw = clamp(*yaw + y * 0.05f, -5, 5);
	player->yaw = *yaw - PV.z * 0.5f;
	MovePlayer(0, 0, player, SS);
}

void	result(float *move_vec, t_player *player)
{
	if (PP.wsad[0] != 0)
	{
		move_vec[0] += player->anglecos * 0.2f;
		move_vec[1] += player->anglesin * 0.2f;
	}
	if (PP.wsad[1] != 0)
	{
		move_vec[0] -= player->anglecos * 0.2f;
		move_vec[1] -= player->anglesin * 0.2f;
	}
	if (PP.wsad[2] != 0)
	{
		move_vec[0] += player->anglesin * 0.2f;
		move_vec[1] -= player->anglecos * 0.2f;
	}
	if (PP.wsad[3] != 0)
	{
		move_vec[0] -= player->anglesin * 0.2f;
		move_vec[1] += player->anglecos * 0.2f;
	}
}

void	move(t_player *player)
{
	int		pushing;
	float	move_vec[2];
	float	acceleration;

	move_vec[0] = 0.f;
	move_vec[1] = 0.f;
	result(move_vec, player);
	if (PP.wsad[0])
		pushing = 1;
	else
		pushing = PP.wsad[1] != 0 || PP.wsad[2] || PP.wsad[3];
	acceleration = pushing ? 0.4 : 0.2;
	player->velocity.x = player->velocity.x * (1 - acceleration)
			+ move_vec[0] * acceleration;
	player->velocity.y = player->velocity.y * (1 - acceleration)
			+ move_vec[1] * acceleration;
	if (pushing)
		PP.moving = 1;
}

void	pos_m(t_system *system, t_player *player, float *eyeheight)
{
	*eyeheight = PP.ducking ? DuckHeight : EyeHeight;
	PP.ground = !PP.falling;
	PP.falling == 1 ? falling(player, system, *eyeheight) : 0;
	PP.moving == 1 ? mvs(player, system, eyeheight, -1) : 0;
}
