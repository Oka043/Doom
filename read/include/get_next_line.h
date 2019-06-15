/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osak <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 11:41:42 by osak              #+#    #+#             */
/*   Updated: 2018/04/19 16:09:29 by osak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# include "../src/libft/libft.h"
# include "player_stable.h"

# define BUFF_SIZE 100

typedef struct		s_string
{
	int				fd;
	char			*s;
	struct s_string	*next;
}					t_string;

int					get_next_line(const int fd, char **line);

#endif