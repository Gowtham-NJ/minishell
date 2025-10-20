/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:41:30 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/17 10:20:59 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_create_t_str(t_str **tail, int len)
{
	t_str	*tmp;

	tmp = malloc(sizeof(t_str));
	if (!tmp)
		return (0);
	tmp->str = malloc(len + 1);
	if (!tmp->str)
	{
		free(tmp);
		return (0);
	}
	tmp->len = len;
	tmp->next = NULL;
	if (*tail)
		(*tail)->next = tmp;
	*tail = tmp;
	return (1);
}

int	ft_cleanup(t_state_split **state_ptr, int free_all)
{
	t_state_split	*state;
	t_str			*tmp;

	if (!state_ptr || !*state_ptr)
		return (0);
	state = *state_ptr;
	while (state->head)
	{
		if (state->head->str)
			free(state->head->str);
		tmp = state->head->next;
		free(state->head);
		state->head = tmp;
	}
	if (free_all)
	{
		if (state->buf)
			free(state->buf);
		free(state);
		*state_ptr = NULL;
	}
	return (1);
}

int	ft_handle_joind_str(t_state_split *state, const char *input_str, int i,
		int j)
{
	char	*str;
	int		k;
	int		l;

	k = 0;
	l = 0;
	str = malloc(state->tail->len + i - j + 1);
	if (!str)
		return (0);
	while (k < state->tail->len)
	{
		str[k] = state->tail->str[k];
		k++;
	}
	while (l < i - j)
		str[k++] = input_str[j + l++];
	str[k] = '\0';
	free(state->tail->str);
	state->tail->str = str;
	state->tail->end = 0;
	state->tail->len += i - j;
	return (1);
}

int	ft_handle_rest_str(t_state_split *state, const char *input_str, int i,
		int j)
{
	int	k;

	if (!state->head || state->tail->end == 1)
	{
		if (!ft_create_t_str(&state->tail, i - j))
			return (ft_cleanup(&state, 0), 0);
		if (!state->head)
			state->head = state->tail;
		k = 0;
		while (k < state->tail->len)
		{
			state->tail->str[k] = input_str[j + k];
			k++;
		}
		state->tail->str[k] = '\0';
		state->tail->end = 0;
	}
	else
		return (ft_handle_joind_str(state, input_str, i, j));
	return (1);
}
