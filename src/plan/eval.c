/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:50:10 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/19 20:09:28 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plan.h"
#include "signal_setup.h"
#include "status.h"
#include <signal.h>

static void	pipeline_close_parent(t_pipeline_plan *plan)
{
	int	index;

	index = 0;
	while (index < plan->parent_close_n)
	{
		close(plan->parent_close[index]);
		index += 1;
	}
}

static void	pipeline_spawn_children(t_exec *exec_ctx, t_pipeline_plan *plan,
		pid_t *pids)
{
	int	index;
	int	count;

	count = plan->nstages;
	index = 0;
	while (index < count)
	{
		pids[index] = fork();
		if (pids[index] == 0)
			run_child_stage(exec_ctx, plan, &plan->stages[index]);
		index += 1;
	}
}

static int	pipeline_wait_last(pid_t *pids, int count)
{
	int	index;
	int	status;
	int	last;

	index = 0;
	last = 1;
	while (index < count)
	{
		waitpid(pids[index], &status, 0);
		if (index == count - 1)
			last = status_from_wait(status);
		index += 1;
	}
	return (last);
}

/**
 * Added sigint ignore in the parent and signal resume after the run.
 */
int	execute_pipeline_plan(t_exec *E, t_pipeline_plan *pp)
{
	int		n;
	int		result;
	pid_t	*pids;

	n = pp->nstages;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pids = (pid_t *)pool_calloc(E->pool, (size_t)n, sizeof(*pids));
	if (!pids)
	{
		signal_prepare_prompt();
		status_set(E, 1);
		return (1);
	}
	pipeline_spawn_children(E, pp, pids);
	pipeline_close_parent(pp);
	result = pipeline_wait_last(pids, n);
	signal_prepare_prompt();
	status_set(E, result);
	return (result);
}
