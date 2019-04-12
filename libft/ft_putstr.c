#include "libft.h"

void	ft_putstr(char const *s)
{
	int n;

	n = 0;
	if (s == NULL)
		return ;
	while (s[n] != '\0')
	{
		write(1, &s[n], 1);
		n++;
	}
}
