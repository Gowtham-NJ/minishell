/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:09:01 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/17 10:20:41 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

# define MAX_FD 1024

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2
# endif

typedef struct s_str
{
	char			*str;
	int				len;
	int				end;
	struct s_str	*next;
}					t_str;

typedef struct s_state_split
{
	t_str			*head;
	t_str			*tail;
	int				fd;
	char			*buf;
}					t_state_split;

int					ft_create_t_str(t_str **tail, int len);
int					ft_cleanup(t_state_split **state_ptr, int free_all);
int					ft_handle_joind_str(t_state_split *state,
						const char *input_str, int i, int j);

int					ft_handle_rest_str(t_state_split *state,
						const char *input_str, int i, int j);
void				ft_n_stplit(t_state_split *state);
char				*ft_consume_line(t_state_split *state);
char				*get_next_line(int fd);

#endif
