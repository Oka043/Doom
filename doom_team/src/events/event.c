/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <osak@student.unit.ua>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 18:11:39 by osak              #+#    #+#             */
/*   Updated: 2019/06/10 18:11:42 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player_stable.h"

void	mvs1(int s, t_sector *sector, float const *eyeh, t_system *system)
{
	float		hl;
	float		hh;
	t_xy		d;
	t_player	*player;

	player = system->player;
	hl = 9e9;
	hh = -9e9;
	if (sector->neighbours[s] >= 0)
	{
		hl = max(sector->floor, SS[sector->neighbours[s]].floor);
		hh = min(sector->ceil, SS[sector->neighbours[s]].ceil);
	}
	if (hh < PW.z + HeadMargin || hl > PW.z - *eyeh + KneeHeight)
	{
		d.x = SEV[s + 1].x - SEV[s + 0].x;
		d.y = SEV[s + 1].y - SEV[s + 0].y;
		PV.x = d.x * (PV.x * d.x + PV.y * d.y) / (SQ(d.x) + SQ(d.y));
		PV.y = d.y * (PV.x * d.x + PV.y * d.y) / (SQ(d.x) + SQ(d.y));
		player->pos.moving = 0;
	}
}

void	mvs(t_player *player, t_system *system, float const *eyeh, int s)
{
	t_sector	*sector;

	sector = &SS[player->num_sect];
	while (++s < sector->npoints)
	{
		if (IntersectBox(PW.x, PW.y, PW.x + PV.x, PW.y + PV.y, SEV[s + 0].x,
				SEV[s + 0].y, SEV[s + 1].x, SEV[s + 1].y)
			&& PointSide(PW.x + PV.x, PW.y + PV.y, SEV[s + 0].x,
					SEV[s + 0].y, SEV[s + 1].x, SEV[s + 1].y) < 0)
		{
			system->player = player;
			mvs1(s, sector, eyeh, system);
			player = system->player;
		}
	}
	MovePlayer(PV.x, PV.y, player, SS);
	player->pos.falling = 1;
}

void	falling(t_player *player, t_system *system, float const eyeheight)
{
	float		nextz;

	PV.z -= 0.05f;
	nextz = PW.z + PV.z;
	if (PV.z < 0 && nextz <
	SS[player->num_sect].floor + eyeheight)
	{
		PW.z = SS[player->num_sect].floor + eyeheight;
		PV.z = 0;
		PP.falling = 0;
		PP.ground = 1;
	}
	else if (PV.z > 0 && nextz > SS[player->num_sect].ceil)
	{
		PV.z = 0;
		PP.falling = 1;
	}
	if (PP.falling)
	{
		PW.z += PV.z;
		PP.moving = 1;
	}
}

int		main_cycle(t_player *player, t_system *system)
{
	SDL_Event	ev;
	float		eyeheight;
	int			skip;
	float		yaw;

	yaw = 0;
	while (1)
	{
		DrawScreen(player, system);
		skip = 0;
		if (++skip >= 1)
		{
			skip = 0;
			SDL_UpdateWindowSurface(SW);
		}
		pos_m(system, player, &eyeheight);
		while (SDL_PollEvent(&ev))
			if (!key_event(&ev, player, system))
				return (0);
//		sprite_casting();
		mouse_event(player, system, &yaw);
		move(player);
		SDL_Delay(10);
	}
}
