/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 01:41:06 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 10:41:47 by vcerny           ###   ########.fr       */
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

static int	prepare_heredocs(t_ctx *ctx, char *line)
{
	int	status;

	ctx->heredoc_count = heredoc_count(line);
	if (ctx->heredoc_count == 0)
		return (0);
	status = heredoc_validate(line);
	if (status != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: Syntax error near unexpected token 'newline'\n");
		ctx->last_status = 2;
		return (-1);
	}
	tcgetattr(STDIN_FILENO, &ctx->snap);
	ctx->heredocs = heredoc_collect(line, ctx->envp, ctx->lines_so_far);
	if (ctx->heredocs == NULL)
	{
		if (signal_flag_get() == SIGINT_HEREDOC)
			ctx->last_status = 130;
		else
			ctx->last_status = 2;
		ctx->heredoc_count = 0;
		return (-1);
	}
	return (0);
}

static int	process_prepare(t_ctx *ctx, char **line, t_exec *exec_ctx,
		int interactive)
{
	ft_bzero(exec_ctx, sizeof(*exec_ctx));
	exec_ctx->envp = &ctx->envp;
	exec_ctx->ctx = ctx;
	exec_ctx->line = *line;
	exec_ctx->last_status = ctx->last_status;
	exec_ctx->trace = 0;
	if (validate_quotes(ctx, line, interactive) != 0)
	{
		free(*line);
		return (ctx->last_status);
	}
	if (interactive && *line && **line)
		add_history(*line);
	if (prepare_heredocs(ctx, *line) != 0)
	{
		free(*line);
		heredoc_release(ctx->heredocs, ctx->heredoc_count);
		ctx->heredocs = NULL;
		ctx->heredoc_count = 0;
		return (ctx->last_status);
	}
	if (ctx->heredoc_count == 0 || signal_flag_get() != SIGINT_HEREDOC)
		ft_add_history_count_lines(ctx, *line);
	return (0);
}

/* stage 2: execute (if allowed), cleanup, and return status */
/* set up a per-command pool for AST/expansion now that
exec_ctx is initialized */
/* dispose per-command pool at end of processing */
int	process_line(t_ctx *ctx, char *line, int interactive)
{
	t_exec	exec_ctx;
	t_pool	exec_pool;
	int		rc;
	int		sig_flag;

	rc = process_prepare(ctx, &line, &exec_ctx, interactive);
	if (rc != 0)
	{
		return (rc);
	}
	pool_init(&exec_pool);
	exec_ctx.pool = &exec_pool;
	sig_flag = signal_flag_get();
	if (sig_flag != SIGINT_HEREDOC && sig_flag != SIGINT_QUOTES)
		executor_run(&exec_ctx, ctx, line);
	heredoc_release(ctx->heredocs, ctx->heredoc_count);
	ctx->heredocs = NULL;
	ctx->heredoc_count = 0;
	free(line);
	pool_dispose(&exec_pool);
	return (ctx->last_status);
}

/* handle one read line; returns:
   0 = keep looping
   1 = EOF in interactive (caller should call ft_handle_minishell_exit)
   2 = EOF non-interactive (caller should return ctx->last_status) */
static int	handle_line(t_ctx *ctx, char *line, int interactive)
{
	if (!line)
	{
		if (interactive)
			return (1);
		return (2);
	}
	if (input_is_blank(line) != 0)
	{
		free(line);
		cleanup_prompt(ctx, interactive);
		return (0);
	}
	ctx->heredocs = NULL;
	ctx->heredoc_count = 0;
	process_line(ctx, line, interactive);
	cleanup_prompt(ctx, interactive);
	return (0);
}

int	shell_loop(t_ctx *ctx)
{
	char	*line;
	int		interactive;
	int		r;
	int		sig_flag;

	interactive = is_interactive();
	if (!interactive)
		signal_prepare_noninteractive();
	while (1)
	{
		signal_flag_clear();
		if (interactive)
			signal_prepare_prompt();
		line = read_one_line(ctx, interactive);
		sig_flag = signal_flag_get();
		if (interactive && sig_flag == SIGINT_MAIN)
			ctx->last_status = 130;
		r = handle_line(ctx, line, interactive);
		if (r == 1)
			cleanup_prompt(ctx, interactive);
		if (r == 1)
			return (ft_handle_minishell_exit(ctx));
		if (r == 2)
			return (ctx->last_status);
	}
}
