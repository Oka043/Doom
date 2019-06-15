/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_length_of_digit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/19 04:28:18 by osak              #+#    #+#             */
/*   Updated: 2017/11/21 12:27:37 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_length_of_digit(long int c)
{
	int i;

	i = 0;
	if (c)
	{
		if (c < 0)
			i++;
		while (c)
		{
			c /= 10;
			i++;
		}
		return (i);
	}
	return (1);
}
