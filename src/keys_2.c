/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvoronyu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 19:39:30 by tvoronyu          #+#    #+#             */
/*   Updated: 2019/01/24 19:46:27 by tvoronyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_pause_2(t_wolf *wolf)
{
    if (wolf->keys[SDL_SCANCODE_ESCAPE] || wolf->sdl.event.type == SDL_QUIT)
        wolf->loop = 0;
//    printf("%d\n", wolf->keys[SDLK_ESCAPE]);
}
