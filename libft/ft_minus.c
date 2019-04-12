#include "libft.h"

void	ft_minus(int *n, int *neg, int *len)
{
	if (*n < 0)
	{
		*n *= -1;
		*neg = 1;
		*len += 1;
	}
}
