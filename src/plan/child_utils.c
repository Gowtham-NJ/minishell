/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 10:53:41 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/19 20:30:54 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "heredoc.h"
#include "minishell.h"
#include "plan.h"
#include "signal_setup.h"

void	ft_def_child_free(t_exec *E)
{
	rl_clear_history();
	free(E->ctx->prompt);
}

void	ft_child_on_err_free(t_exec *E)
{
	ft_free_env_copy(E->ctx->envp);
	pool_dispose(E->pool);
}

int	ft_child_stage_cmd(t_exec *E, t_child_plan *cp)
{
	t_cmd	*c;
	int		k;
	char	**argv;

	c = cp->cmd;
	k = assign_prefix_len(c->argv);
	argv = c->argv + k;
	if (!argv[0])
	{
		ft_child_on_err_free(E);
		exit(0);
	}
	if (is_builtin(argv[0]))
		run_builtin_child(E, argv);
	return (exec_with_resolution(E, argv));
}
