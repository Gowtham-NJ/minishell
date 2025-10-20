/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:18:12 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 10:13:49 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	pipe_write_all(int fd, const char *body)
{
	size_t	len;
	size_t	written;
	ssize_t	res;

	len = ft_strlen(body);
	written = 0;
	while (written < len)
	{
		res = write(fd, body + written, len - written);
		if (res > 0)
			written += (size_t)res;
		else if (res < 0 && errno == EINTR)
			continue ;
		else
			return (-1);
	}
	return (0);
}

int	executor_attach_heredocs(t_exec *E, t_ast *root, t_heredoc *docs,
		int doc_count)
{
	int		index;
	int		iter;
	t_redir	**targets;

	if (doc_count <= 0)
		return (0);
	targets = (t_redir **)pool_calloc(E->pool, (size_t)doc_count,
			sizeof(t_redir *));
	if (!targets)
		return (-1);
	index = 0;
	if (collect_targets(root, targets, doc_count, &index) != 0
		|| index != doc_count)
	{
		return (-1);
	}
	iter = 0;
	while (iter < doc_count)
	{
		if (attach_single_doc(targets[iter], &docs[iter]) != 0)
			return (-1);
		iter += 1;
	}
	return (0);
}

static void	exec_after_parse(t_exec *e, t_ctx *c, t_ast *root, t_parser *p)
{
	int	status;

	if (executor_attach_heredocs(e, root, c->heredocs, c->heredoc_count) != 0)
	{
		tok_free(&p->tv);
		executor_free_ast(root);
		status_set(e, 1);
		c->last_status = 1;
		return ;
	}
	tok_free(&p->tv);
	if (expand_exit_status(e, root) != 0 || expand_env_vars_in_ast(e, root) != 0
		|| expand_ast(root) != 0)
	{
		executor_free_ast(root);
		status_set(e, 1);
		c->last_status = 1;
		return ;
	}
	status = run_ast(e, root);
	executor_free_ast(root);
	status_set(e, status);
	c->last_status = status & 255;
}

/* helper: print/clean on parse error and set statuses */
static void	report_parse_error(t_exec *exec_ctx, t_ctx *ctx, t_ex_run *ex)
{
	if (ex->p.err_tok)
		ex->tok = ex->p.err_tok;
	if (ex->p.err_msg && ft_strcmp((char *)ex->p.err_msg,
			"unexpected t_token") == 0)
		syntax_error_unexpected(ex->tok);
	else
		ft_dprintf(2, "minishell: syntax error near '%s'\n", ex->tok);
	free(ex->p.err_tok_mem);
	ex->p.err_tok_mem = NULL;
	tok_free(&ex->p.tv);
	status_set(exec_ctx, 2);
	ctx->last_status = 2;
}

void	executor_run(t_exec *exec_ctx, t_ctx *ctx, const char *line)
{
	t_ex_run	ex;

	if (ctx)
		ctx->last_status = 1;
	if (!exec_ctx || !ctx)
		return ;
	ex.p.ast_pool = exec_ctx->pool;
	ex.tok = "newline";
	ex.root = parse_line_to_ast(&ex.p, line);
	if (!ex.root)
	{
		report_parse_error(exec_ctx, ctx, &ex);
		return ;
	}
	exec_ctx->ast = ex.root;
	exec_after_parse(exec_ctx, ctx, ex.root, &ex.p);
}
