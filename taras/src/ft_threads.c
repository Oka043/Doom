//
// Created by Taras VORONYUK on 2019-03-03.
//

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvoronyu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/19 13:58:45 by tvoronyu          #+#    #+#             */
/*   Updated: 2018/08/19 14:00:06 by tvoronyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	*ft_comand_threads(void *wolf)
{
    t_threads	*potok;
    int			x;
    int			x_max;

    potok = (t_threads*)wolf;
    x = (WIDTH / N) * potok->n;
    x_max = WIDTH / N * (potok->n + 1);
//    ft_render(wolf);
//    ft_render(potok->wolf, x , x_max);
    pthread_exit(0);
}

void	ft_threads(t_wolf *wolf)
{
    t_threads		potok[N];
    int				i;
    int				n;

    n = 4;
    i = -1;
    while (++i < N)
    {
        --n;
        potok[i].wolf = wolf;
        potok[i].n = n;
        pthread_create(&potok[i].threads, NULL, ft_comand_threads, &potok[i]);
    }
    i = -1;
    while (++i < N)
        pthread_join(potok[i].threads, NULL);
}
