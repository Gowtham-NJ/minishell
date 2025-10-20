/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_plan_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 20:15:54 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/19 20:27:49 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plan.h"

int	ft_plan_close_pipes(int pipes[][2], int *i)
{
	perror("pipe");
	while (--(*i) >= 0)
	{
		close(pipes[*i][0]);
		close(pipes[*i][1]);
	}
	return (-1);
}

int	build_pipeline_plan_2(t_exec *E, t_pipeline_plan *pp, t_plan *p)
{
	int	i;

	pp->stages = pool_calloc(E->pool, p->n, sizeof(*pp->stages));
	if (!pp->stages)
		return (-1);
	ft_memset(pp->stages, 0, p->n * sizeof(*pp->stages));
	pp->nstages = p->n;
	pp->pipes = p->pipes;
	if (make_pipes_and_parent_close(E, p->n, p->pipes, pp) < 0)
		return (-1);
	i = 0;
	while (i < p->n)
	{
		init_stage_core(pp, i, p->units[i], p->pipes);
		init_stage_fds_and_redirs(pp, i, p->units[i], p->pipes);
		i++;
	}
	return (0);
}
