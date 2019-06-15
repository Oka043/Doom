/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 16:18:18 by osak              #+#    #+#             */
/*   Updated: 2017/11/21 11:45:39 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *destination, int c, size_t n)
{
	unsigned char	*byte;
	size_t			u;

	u = 0;
	while (u < n)
	{
		byte = (unsigned char*)destination;
		byte[u] = (char)c;
		u++;
	}
	return (destination);
}
