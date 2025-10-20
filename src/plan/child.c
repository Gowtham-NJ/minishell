/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:50:45 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/19 20:06:44 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "plan.h"
#include "signal_setup.h"

static int	eval_flow(t_exec *E, t_ast *n);

static int	eval_node_cmd_child(t_exec *E, t_cmd *c)
{
	int		k;
	char	**argv;

	k = assign_prefix_len(c->argv);
	argv = c->argv + k;
	if (!argv[0])
		return (0);
	if (is_builtin(argv[0]))
		return (run_builtin_child(E, argv));
	return (exec_with_resolution(E, argv));
}

int	eval_node_child(t_exec *E, t_ast *n)
{
	t_pipeline_plan	pp;
	int				s;

	if (n->kind == NK_CMD)
		return (eval_node_cmd_child(E, n->cmd));
	if (n->kind == NK_GROUP)
		return (eval_flow(E, n->child));
	if (n->kind == NK_PIPE)
	{
		ft_memset(&pp, 0, sizeof(pp));
		build_pipeline_plan(E, n, &pp);
		s = execute_pipeline_plan(E, &pp);
		return (s);
	}
	return (eval_flow(E, n));
}

static int	eval_flow(t_exec *E, t_ast *n)
{
	int	s;

	if (n->kind == NK_LIST)
	{
		(void)eval_node_child(E, n->left);
		return (eval_node_child(E, n->right));
	}
	if (n->kind == NK_AND)
	{
		s = eval_node_child(E, n->left);
		if (s == 0)
			return (eval_node_child(E, n->right));
		return (s);
	}
	if (n->kind == NK_OR)
	{
		s = eval_node_child(E, n->left);
		if (s != 0)
			return (eval_node_child(E, n->right));
		return (s);
	}
	return (1);
}

static void	child_setup_io(t_exec *E, t_pipeline_plan *pp, t_child_plan *cp)
{
	int	i;

	if (cp->in_fd != -1)
	{
		if (E->trace)
			ft_dprintf(2, "[child %d] dup2(%d->0)\n", cp->index, cp->in_fd);
		dup2(cp->in_fd, 0);
	}
	if (cp->out_fd != -1)
	{
		if (E->trace)
			ft_dprintf(2, "[child %d] dup2(%d->1)\n", cp->index, cp->out_fd);
		dup2(cp->out_fd, 1);
	}
	i = 0;
	while (i < pp->nstages - 1)
	{
		close(pp->pipes[i][1]);
		close(pp->pipes[i][0]);
		i++;
	}
}

/* Match bash: ignore SIGPIPE so writers emit EPIPE diagnostics. */
/**
 * if we continue ignoring sigpipe, stuff like "yes | head -n 1" does not work
 *
 * Added default signals for the children
 */
void	run_child_stage(t_exec *E, t_pipeline_plan *pp, t_child_plan *cp)
{
	int		t;
	t_pool	local;

	ft_def_child_free(E);
	signal_prepare_child();
	child_setup_io(E, pp, cp);
	t = apply_redirs(E, cp->redirs);
	if (t != 0)
	{
		ft_child_on_err_free(E);
		exit(1);
	}
	if (cp->k == STAGE_NODE_CMD)
		t = ft_child_stage_cmd(E, cp);
	else
		t = eval_node_child(E, cp->node);
	ft_child_on_err_free(E);
	exit(t);
}
