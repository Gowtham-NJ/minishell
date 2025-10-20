/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:21:57 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/20 18:32:46 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "executor.h"
#include "signal_setup.h"
#include "status.h"
#include "minishell.h"

/* child-only path: never returns */
static void	run_child_simple(t_exec *E, t_cmd *c, char **argv)
{
	t_pool	local;
	int		rc;

	ft_def_child_free(E);
	signal_prepare_child();
	rc = apply_redirs(E, c->redirs);
	if (rc != 0)
	{
		ft_child_on_err_free(E);
		exit(1);
	}
	rc = exec_with_resolution(E, argv);
	ft_child_on_err_free(E);
	exit(rc);
}

/* external command: fork, apply redirs in child, exec */
/* isolate allocations in the child */
/**
 * Added default signals in children
 */
int	spawn_simple_child(t_exec *E, t_cmd *c, char **argv)
{
	pid_t	pid;
	int		st;
	int		rc;

	pid = fork();
	if (pid == 0)
		run_child_simple(E, c, argv);
	else if (pid < 0)
	{
		status_set(E, 1);
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &st, 0);
	signal_prepare_prompt();
	rc = status_from_wait(st);
	status_set(E, rc);
	return (rc);
}

static int	eval_binary(t_exec *E, t_ast *n)
{
	int	s;

	if (n->kind == NK_LIST)
	{
		(void)eval_node(E, n->left);
		return (eval_node(E, n->right));
	}
	if (n->kind == NK_AND)
	{
		s = eval_node(E, n->left);
		if (s == 0)
			return (eval_node(E, n->right));
		return (s);
	}
	if (n->kind == NK_OR)
	{
		s = eval_node(E, n->left);
		if (s != 0)
			return (eval_node(E, n->right));
		return (s);
	}
	return (1);
}

int	eval_node(t_exec *e, t_ast *n)
{
	if (!n)
		return (0);
	if (n->kind == NK_CMD)
		return (eval_cmd_node(e, n));
	if (n->kind == NK_GROUP)
		return (eval_group_no_subshell(e, n));
	if (n->kind == NK_PIPE)
		return (executor_eval_pipeline(e, n));
	return (eval_binary(e, n));
}

int	run_ast(t_exec *E, t_ast *n)
{
	int	st;

	st = eval_node(E, n);
	status_set(E, st);
	return (st);
}
