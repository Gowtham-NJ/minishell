/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmdsubst_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:12:04 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 16:57:35 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"
#include "signal_setup.h"
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

static void	cs_print_parse(const t_parser *p, const char *tok)
{
	if (p->err_msg && ft_strcmp((char *)p->err_msg, "unexpected t_token") == 0)
		syntax_error_unexpected(tok);
	else if (p->err_msg && ft_strcmp((char *)p->err_msg,
			"expected command") == 0 && (!tok || ft_strcmp(tok, "newline") == 0
			|| !ft_isalpha((unsigned char)tok[0])))
		syntax_error_unexpected(tok);
	else if (p->err_msg && ft_strcmp((char *)p->err_msg, "unclosed quote") == 0)
		ft_dprintf(2, "minishell: %s\n", p->err_msg);
	else if (p->err_msg)
		ft_dprintf(2, "minishell: %s near '%s'\n", p->err_msg, tok);
	else
		ft_dprintf(2, "minishell: syntax error near '%s'\n", tok);
}

static t_ast	*cs_child_parse_or_die(const char *body, t_parser *p,
		t_pool *pool)
{
	t_ast		*root;
	const char	*tok;

	ft_memset(p, 0, sizeof(*p));
	p->ast_pool = pool;
	root = parse_line_to_ast(p, body);
	if (root)
		return (root);
	tok = "newline";
	if (p->err_tok)
		tok = p->err_tok;
	cs_print_parse(p, tok);
	free(p->err_tok_mem);
	p->err_tok_mem = NULL;
	tok_free(&p->tv);
	return (NULL);
}

/* worker: parse → expand → run; returns 0..255 on success or parse error;
   exits immediately on expand error to match original behavior. */
static int	cs_child_run(t_exec *ex, const char *body)
{
	t_parser	p;
	t_ast		*root;
	int			status;

	root = cs_child_parse_or_die(body, &p, ex->pool);
	if (!root)
		return (2);
	tok_free(&p.tv);
	if (expand_exit_status(ex, root) != 0 || expand_env_vars_in_ast(ex,
			root) != 0 || expand_ast(root) != 0)
	{
		executor_free_ast(root);
		exit(1);
	}
	status = run_ast(ex, root);
	executor_free_ast(root);
	return (status & 0xFF);
}

void	command_subst_exec_child(t_exec *exec_ctx, const char *body,
		int write_fd)
{
	t_exec	ex;
	int		status;
	t_pool	local;

	ex = *exec_ctx;
	ex.ast = NULL;
	ex.sv = NULL;
	ex.line = NULL;
	ex.prompt = NULL;
	signal_prepare_child();
	ex.pool = &local;
	pool_init(ex.pool);
	if (dup2(write_fd, STDOUT_FILENO) < 0)
	{
		pool_dispose(ex.pool);
		exit(1);
	}
	close(write_fd);
	status = cs_child_run(&ex, body);
	pool_dispose(ex.pool);
	exit(status);
}
