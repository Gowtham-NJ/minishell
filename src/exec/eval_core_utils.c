/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_core_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 11:08:34 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 12:56:06 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "executor.h"
#include "status.h"

/* builtin in parent: honor redirs, run once, restore fds */
int	run_builtin_with_redirs(t_exec *E, t_cmd *c, char **argv)
{
	t_fd_save	sv;
	int			rc;

	sv.in = dup(0);
	sv.out = dup(1);
	sv.err = dup(2);
	rc = apply_redirs(E, c->redirs);
	if (rc != 0)
	{
		fd_save_end(&sv);
		status_set(E, 1);
		return (1);
	}
	E->sv = &sv;
	rc = run_builtin_parent(E, argv);
	fd_save_end(&sv);
	status_set(E, rc);
	return (rc);
}

/* apply_redirs_only
** Validate/apply redirections when there is no command word.
** Sets status to 0 on success, 1 on failure; returns the same code.
*/
static int	apply_redirs_only(t_exec *e, t_cmd *c)
{
	t_fd_save	sv;
	int			rc;

	sv.in = dup(0);
	sv.out = dup(1);
	sv.err = dup(2);
	rc = apply_redirs(e, c->redirs);
	fd_save_end(&sv);
	if (rc == 0)
	{
		status_set(e, 0);
		return (0);
	}
	status_set(e, 1);
	return (1);
}

/* export_assignments
** Persist NAME=VALUE words into the shell environment; sets status to 0.
*/
static int	export_assignments(t_exec *e, char **argv)
{
	int		i;
	char	*tmp[3];

	i = 0;
	tmp[0] = (char *)"export";
	tmp[2] = NULL;
	while (argv[i])
	{
		tmp[1] = argv[i];
		(void)ft_export(e->envp, tmp);
		i++;
	}
	status_set(e, 0);
	return (0);
}

/* eval_node
** Dispatch by AST kind. For NK_CMD: skip assignment prefix; if empty,
** apply redirs or export assignments; else run builtin or external.
*/
int	eval_cmd_node(t_exec *e, t_ast *n)
{
	char	**argv;
	int		k;

	if (!n->cmd || !n->cmd->argv)
		return (0);
	k = assign_prefix_len(n->cmd->argv);
	argv = n->cmd->argv + k;
	if (!argv[0])
	{
		if (n->cmd->redirs)
			return (apply_redirs_only(e, n->cmd));
		return (export_assignments(e, n->cmd->argv));
	}
	if (is_builtin(argv[0]))
		return (run_builtin_with_redirs(e, n->cmd, argv));
	return (spawn_simple_child(e, n->cmd, argv));
}

t_ast	*node(t_pool *pool, t_node_kind k)
{
	t_ast	*n;

	n = (t_ast *)pool_calloc(pool, 1, sizeof(*n));
	if (!n)
	{
		perror("malloc");
		exit(1);
	}
	n->kind = k;
	return (n);
}
