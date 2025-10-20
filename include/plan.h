/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:29 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/20 18:44:22 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft/libft.h"
#include "proof.h"

/** for norminette's sake */
typedef struct s_plan
{
	int		n;
	t_ast	**units;
	int		out_n;
	int		(*pipes)[2];
}			t_plan;

int			execute_pipeline_plan(t_exec *E, t_pipeline_plan *pp);
void		run_child_stage(t_exec *E, t_pipeline_plan *pp, t_child_plan *cp);
t_redir		*merge_redirs(t_redir *a, t_redir *b);
int			ft_child_stage_cmd(t_exec *E, t_child_plan *cp);
int			ft_plan_close_pipes(int pipes[][2], int *i);
int			build_pipeline_plan_2(t_exec *E, t_pipeline_plan *pp, t_plan *p);
int			make_pipes_and_parent_close(t_exec *E, int n, int pipes[][2],
				t_pipeline_plan *pp);
void		init_stage_fds_and_redirs(t_pipeline_plan *pp, int i, t_ast *u,
				int pipes[][2]);
void		init_stage_core(t_pipeline_plan *pp, int i, t_ast *u,
				int pipes[][2]);
