/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:41:21 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/17 10:20:49 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_n_stplit(t_state_split *state)
{
	int	br;
	int	i;
	int	j;

	while (!(state->tail && state->tail->end == 1))
	{
		br = read(state->fd, state->buf, BUFFER_SIZE);
		if (br <= 0)
			break ;
		state->buf[br] = '\0';
		i = 0;
		j = 0;
		while (state->buf[i])
		{
			if (state->buf[i] == '\n')
			{
				ft_handle_rest_str(state, state->buf, i + 1, j);
				state->tail->end = 1;
				j = i + 1;
			}
			i++;
		}
		if (j < i)
			ft_handle_rest_str(state, state->buf, i, j);
	}
}

char	*ft_consume_line(t_state_split *state)
{
	t_str	*tmp;
	char	*line;
	int		i;

	line = malloc(state->head->len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (state->head->str[i])
	{
		line[i] = state->head->str[i];
		i++;
	}
	line[i] = '\0';
	tmp = state->head;
	state->head = tmp->next;
	free(tmp->str);
	free(tmp);
	return (line);
}

static int	ft_init_state(t_state_split **state_ptr_array, int fd)
{
	t_state_split	*state;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (0);
	if (state_ptr_array[fd] == NULL)
	{
		state = malloc(sizeof(t_state_split));
		if (!state)
			return (0);
		state->head = NULL;
		state->tail = NULL;
		state->fd = fd;
		state->buf = malloc(BUFFER_SIZE + 1);
		if (!state->buf)
		{
			free(state);
			return (0);
		}
		state_ptr_array[fd] = state;
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_state_split	*states[MAX_FD] = {NULL};
	t_state_split			*state;
	char					*line;

	if (!ft_init_state(states, fd))
		return (NULL);
	state = states[fd];
	ft_n_stplit(state);
	if (!state || !state->head || !state->head->str)
	{
		ft_cleanup(&states[fd], 1);
		return (NULL);
	}
	line = ft_consume_line(state);
	if (!line)
	{
		ft_cleanup(&states[fd], 1);
		return (NULL);
	}
	if (!state->head)
		ft_cleanup(&states[fd], 1);
	return (line);
}
