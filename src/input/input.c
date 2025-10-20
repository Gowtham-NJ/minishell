/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:47:15 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/10 18:47:17 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "libft/libft.h"

static int	quote_next_state(int state, char ch)
{
	if (state == 1 && ch == '\'')
		return (0);
	if (state == 2 && ch == '"')
		return (0);
	if (state == 0 && ch == '\'')
		return (1);
	if (state == 0 && ch == '"')
		return (2);
	return (state);
}

int	input_check_quotes(const char *line)
{
	int	state;
	int	i;

	state = 0;
	i = 0;
	while (line[i] != '\0')
	{
		state = quote_next_state(state, line[i]);
		i += 1;
	}
	if (state != 0)
		return (-1);
	return (0);
}

int	input_is_blank(const char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (ft_isspace((int)line[i]) == 0)
			return (0);
		i += 1;
	}
	return (1);
}
