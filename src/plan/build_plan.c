/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_plan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:52:17 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/20 19:09:30 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plan.h"

// Returns total number of units. If arr && out_n are provided, it fills arr.
static int	collect_pipe_units(const t_ast *n, t_ast **arr, int *out_n)
{
	int	l;
	int	r;

	if (!n)
		return (0);
	if (n->kind != NK_PIPE)
	{
		if (arr && out_n)
			arr[(*out_n)++] = (t_ast *)n;
		return (1);
	}
	l = collect_pipe_units(n->left, arr, out_n);
	r = collect_pipe_units(n->right, arr, out_n);
	return (l + r);
}

/* cleanup any previously opened pipes to avoid FD leaks */
int	make_pipes_and_parent_close(t_exec *E, int n, int pipes[][2],
		t_pipeline_plan *pp)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipes[i]) < 0)
			return (ft_plan_close_pipes(pipes, &i));
		i++;
	}
	pp->parent_close = pool_calloc(E->pool, 2 * (n - 1),
			sizeof(*pp->parent_close));
	if (!pp->parent_close)
		return (-1);
	pp->parent_close_n = 0;
	i = 0;
	while (i < n - 1)
	{
		pp->parent_close[pp->parent_close_n++] = pipes[i][0];
		pp->parent_close[pp->parent_close_n++] = pipes[i][1];
		i++;
	}
	return (0);
}

void	init_stage_core(t_pipeline_plan *pp, int i, t_ast *u, int pipes[][2])
{
	t_child_plan	*cp;

	cp = &pp->stages[i];
	cp->index = i;
	cp->total = pp->nstages;
	if (u->kind == NK_CMD)
	{
		cp->k = STAGE_NODE_CMD;
		cp->cmd = u->cmd;
	}
	else
	{
		cp->k = STAGE_NODE_GROUP;
		cp->node = u;
	}
	if (i == 0)
		cp->in_fd = -1;
	else
		cp->in_fd = pipes[i - 1][0];
	if (i == pp->nstages - 1)
		cp->out_fd = -1;
	else
		cp->out_fd = pipes[i][1];
	cp->close_count = 0;
}

/* no longer precompute close_list; child will iterate plan->pipes */
void	init_stage_fds_and_redirs(t_pipeline_plan *pp, int i, t_ast *u,
		int pipes[][2])
{
	t_redir			*cmdr;
	t_child_plan	*cp;
	t_redir			*groupr;

	(void)pipes;
	cp = &pp->stages[i];
	cp->close_count = 0;
	if (u->kind == NK_GROUP)
		groupr = u->redirs;
	else
		groupr = NULL;
	if (u->kind == NK_CMD)
		cmdr = u->cmd->redirs;
	else
		cmdr = NULL;
	cp->redirs = merge_redirs(groupr, cmdr);
}

int	build_pipeline_plan(t_exec *E, t_ast *node, t_pipeline_plan *pp)
{
	t_plan	p;
	size_t	s;

	p.n = collect_pipe_units(node, NULL, NULL);
	if (p.n <= 0)
		return (-1);
	p.units = (t_ast **)pool_calloc(E->pool, (size_t)p.n, sizeof(*p.units));
	if (!p.units)
		return (-1);
	p.pipes = NULL;
	s = (size_t)(p.n - 1);
	if (p.n > 1)
	{
		p.pipes = (int (*)[2])pool_calloc(E->pool, s, sizeof(*p.pipes));
		if (!p.pipes)
			return (-1);
	}
	p.out_n = 0;
	if (collect_pipe_units(node, p.units, &p.out_n) != p.n || p.out_n != p.n)
		return (-1);
	return (build_pipeline_plan_2(E, pp, &p));
}
