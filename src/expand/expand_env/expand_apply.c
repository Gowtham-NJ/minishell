/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_apply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:00:12 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:49:29 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"

static int	redir_apply_matches(char **ptarget, char **ms, size_t n)
{
	char	*newt;

	if (n == 0)
		return (0);
	if (n == 1)
	{
		newt = ms[0];
		*ptarget = newt;
		return (0);
	}
	ft_dprintf(STDERR_FILENO, "minishell: ambiguous redirect\n");
	return (-2);
}

/* unchanged signature */
static int	apply_glob_redir(t_pool *pool, char **ptarget)
{
	char		*dir;
	const char	*pat;
	char		**ms;
	ssize_t		k;
	int			r;

	dir = NULL;
	pat = NULL;
	r = redir_dir_pat(pool, *ptarget, &dir, &pat);
	if (r <= 0)
	{
		if (r < 0)
			return (-1);
		return (0);
	}
	k = collect_matches(pool, dir, pat, &ms);
	if (k < 0)
		return (-1);
	return (redir_apply_matches(ptarget, ms, (size_t)k));
}

/* ------ helpers for expand_env_vars_in_cmd --------------- */

static int	expand_one_redir(t_exec *E, t_redir *r)
{
	char	*nw;
	int		dq;
	int		want_glob;
	int		gr;

	want_glob = (r->kind != R_HDOC) && has_unquoted_glob(r->target);
	nw = expand_word_env(E, r->target, *E->envp, &dq);
	if (!nw)
		return (-1);
	r->target = nw;
	if (!want_glob)
		return (0);
	gr = apply_glob_redir(E->pool, &r->target);
	if (gr == -2)
		return (-2);
	if (gr < 0)
		return (-1);
	return (0);
}

/* original signature, behavior preserved */
static int	expand_env_vars_in_cmd(t_exec *exec_ctx, t_ast *node)
{
	t_redir	*r;
	int		rr;

	if (!exec_ctx || !node)
		return (-1);
	if (!node->cmd)
		return (0);
	if (expand_env_in_cmd(exec_ctx, node->cmd) != 0)
		return (-1);
	r = node->cmd->redirs;
	while (r)
	{
		rr = expand_one_redir(exec_ctx, r);
		if (rr != 0)
			return (rr);
		r = r->next;
	}
	return (0);
}

int	expand_env_vars_in_ast(t_exec *exec_ctx, t_ast *node)
{
	int	rr;
	int	lr;

	if (!exec_ctx)
		return (-1);
	if (!node)
		return (0);
	if (node->kind == NK_CMD)
		return (expand_env_vars_in_cmd(exec_ctx, node));
	if (node->kind == NK_GROUP)
		return (expand_env_vars_in_ast(exec_ctx, node->child));
	lr = expand_env_vars_in_ast(exec_ctx, node->left);
	if (lr != 0)
		return (lr);
	rr = expand_env_vars_in_ast(exec_ctx, node->right);
	if (rr != 0)
		return (rr);
	return (0);
}
