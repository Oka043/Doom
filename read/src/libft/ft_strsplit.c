/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/21 10:50:23 by osak              #+#    #+#             */
/*   Updated: 2017/11/21 14:36:54 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_count(char const *s, char c)
{
	size_t i;

	i = 0;
	while (*s)
	{
		if ((*s) != c)
		{
			i++;
			while (*s != c && *s)
				s++;
		}
		if (*s)
			s++;
	}
	return (i);
}

static char		*get_word(char const **s, char c)
{
	char *end;
	char *ret_str;

	ret_str = 0;
	while (**s)
	{
		if (**s != c)
		{
			end = ft_strchr(*s, c);
			if (!end)
				return (ft_strdup(*s));
			ret_str = ft_strnew(end - (*s));
			if (ret_str)
			{
				ft_memmove(ret_str, *s, end - (*s));
				*s = end;
				break ;
			}
		}
		(*s)++;
	}
	return (ret_str);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**ret_arr;
	char	*str;
	size_t	word_count;
	size_t	i;

	word_count = 0;
	ret_arr = 0;
	if (s && c >= 0)
	{
		word_count = get_count(s, c);
		ret_arr = (char **)ft_memalloc(sizeof(char *) * (word_count + 1));
		if (ret_arr)
		{
			i = -1;
			while (++i < word_count)
			{
				str = get_word(&s, c);
				if (str)
					ret_arr[i] = str;
			}
			ret_arr[i] = 0;
		}
	}
	return (ret_arr);
}
