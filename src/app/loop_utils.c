/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:10:38 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/14 16:09:49 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_history_count_lines(t_ctx *ctx, char *line)
{
	ctx->lines_so_far += ft_count_lines(line);
}

int	ft_count_lines(const char *line)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	if (line && line[0] != '\0')
		res++;
	while (line[i])
	{
		if (line[i] == '\n')
			res++;
		i++;
	}
	return (res);
}

/**
 * Handles shell exit
 */
int	ft_handle_minishell_exit(t_ctx *ctx)
{
	rl_clear_history();
	return (ctx->last_status);
}
