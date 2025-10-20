/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:22:03 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 12:59:00 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "status.h"

/* arrays are exec-pool-owned; nothing to free explicitly */
static void	pipeline_plan_release(t_pipeline_plan *plan)
{
	if (!plan)
		return ;
	plan->stages = NULL;
	plan->parent_close = NULL;
}

/*
** executor_eval_pipeline: execute a pipeline and propagate its status.
** Inputs: execution context and pipeline AST node.
** Outputs: returns the collected exit status from the pipeline.
** Side effects: frees planning buffers and updates the context on failure.
*/
int	executor_eval_pipeline(t_exec *exec_ctx, t_ast *node)
{
	t_pipeline_plan	plan;
	int				status;

	ft_memset(&plan, 0, sizeof(plan));
	if (build_pipeline_plan(exec_ctx, node, &plan) != 0)
	{
		pipeline_plan_release(&plan);
		status_set(exec_ctx, 1);
		return (1);
	}
	status = execute_pipeline_plan(exec_ctx, &plan);
	pipeline_plan_release(&plan);
	return (status);
}
