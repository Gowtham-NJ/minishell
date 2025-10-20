/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:18:59 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 12:26:10 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "executor.h"
#include "ft_quotes.h"
#include "heredoc.h"
#include "input.h"
#include "loop_helper.h"
#include "minishell.h"
#include "signal_setup.h"

/* Interactive only when BOTH stdin and stdout are TTYs */
int	is_interactive(void)
{
	if (!isatty(STDIN_FILENO))
		return (1);
	if (!isatty(STDOUT_FILENO))
		return (1);
	return (1);
}

char	*read_one_line(t_ctx *ctx, int interactive)
{
	char	*line;
	size_t	len;

	if (interactive)
	{
		ctx->prompt = ft_make_prompt(&ctx->envp);
		line = readline(ctx->prompt);
		return (line);
	}
	line = get_next_line(STDIN_FILENO);
	if (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
	return (line);
}

void	cleanup_prompt(t_ctx *ctx, int interactive)
{
	if (!interactive)
		return ;
	free(ctx->prompt);
	ctx->prompt = NULL;
}

int	validate_quotes(t_ctx *ctx, char **line, int interactive)
{
	int		result;
	char	*prompt;

	result = input_check_quotes(*line);
	if (result == 0)
		return (0);
	if (!interactive)
		return (ctx->last_status = 2, -1);
	result = ft_quotes_allow_more(line, ctx->prompt, ctx->envp);
	if (result == 0)
		return (0);
	else if (result != 0 && signal_flag_get() == SIGINT_QUOTES)
	{
		ctx->last_status = 130;
		return (-1);
	}
	ctx->last_status = 2;
	return (-1);
}
