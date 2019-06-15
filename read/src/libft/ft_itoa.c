/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/19 02:28:09 by osak              #+#    #+#             */
/*   Updated: 2017/11/21 16:08:43 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define ABS(x) (((x) > 0) ? (x) : -(x))

char	*ft_itoa(int k)
{
	char			*str;
	int				u;
	unsigned int	re;

	u = ft_length_of_digit(k);
	re = ABS(k);
	str = ft_strnew(u);
	if (str)
	{
		if (k == 0)
			str[0] = '0';
		else
		{
			while (u >= 0)
			{
				str[--u] = (re % 10) + '0';
				re = ABS(re) / 10;
			}
			if (k < 0)
				*str = '-';
		}
	}
	return (str);
}
