/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:30:59 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:09:30 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	append_to_list(t_state *state, int end, t_bool type)
{
	t_printf	*new_node;

	new_node = malloc(sizeof(t_printf));
	state->end = end;
	if (!new_node)
		return ;
	new_node->input = ft_strndup(state->str + state->start, state->end
			- state->start);
	new_node->type = type;
	new_node->flags = NULL;
	new_node->next = NULL;
	new_node->include = t_false;
	if (type == t_true && ft_strchr("diouxXcspn%", state->str[end - 1]))
		new_node->include = t_true;
	if (!state->head)
	{
		state->head = new_node;
		state->tail = new_node;
	}
	else
	{
		state->tail->next = new_node;
		state->tail = new_node;
	}
	state->start = end;
}

static int	is_flag_char(char c)
{
	return (ft_strchr("0123456789.-+ #hlLzjt", c) != NULL);
}

void	parse_printf_string(t_state *state)
{
	int	i;
	int	j;

	i = 0;
	while (state->str[i])
	{
		if (state->str[i] == '%')
		{
			if (state->start != i)
				append_to_list(state, i, t_false);
			j = i + 1;
			while (state->str[j] && !ft_strchr("diouxXfFeEgGaAcspn%",
					state->str[j]) && is_flag_char(state->str[j]))
				j++;
			if (state->str[j])
			{
				append_to_list(state, j + 1, t_true);
				i = j + 1;
				continue ;
			}
		}
		i++;
	}
	if (state->start < i)
		append_to_list(state, i, t_false);
}

int	build_output(t_printf *node)
{
	int	numb_arg;

	numb_arg = 0;
	while (node)
	{
		if (node->type && node->include)
		{
			node->flags = malloc(sizeof(t_flags));
			if (node->flags)
			{
				node->flags = ft_find_flags_id((const char *)node->input);
				numb_arg++;
			}
		}
		node = node->next;
	}
	return (numb_arg);
}
