/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:20:51 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 17:29:46 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_ast	*ast_and(t_pool *pool, t_ast *a, t_ast *b)
{
	t_ast	*n;

	n = node(pool, NK_AND);
	n->left = a;
	n->right = b;
	return (n);
}

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp((char *)name, "echo") == 0)
		return (1);
	if (ft_strcmp((char *)name, "cd") == 0)
		return (1);
	if (ft_strcmp((char *)name, "export") == 0)
		return (1);
	if (ft_strcmp((char *)name, "env") == 0)
		return (1);
	if (ft_strcmp((char *)name, "unset") == 0)
		return (1);
	if (ft_strcmp((char *)name, "pwd") == 0)
		return (1);
	if (ft_strcmp((char *)name, "true") == 0)
		return (1);
	if (ft_strcmp((char *)name, "false") == 0)
		return (1);
	if (ft_strcmp((char *)name, "exit") == 0)
		return (1);
	return (0);
}

int	run_builtin_parent(t_exec *E, char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, E->envp, E->pool));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (bi_pwd(argv));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv, E->envp, E->pool));
	if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(argv, *E->envp));
	if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(E->envp, argv));
	if (ft_strcmp(argv[0], "true") == 0)
		return (0);
	if (ft_strcmp(argv[0], "false") == 0)
		return (1);
	if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit_builtin(E, argv, 0));
	return (127);
}

/* ft_exit_builtin handles pool_dispose in child mode */
/* dispose pool before exiting the child after builtin */
int	run_builtin_child(t_exec *E, char **argv)
{
	int	st;

	if (argv && argv[0] && ft_strcmp(argv[0], "exit") == 0)
	{
		st = ft_exit_builtin(E, argv, 1);
		exit(st);
	}
	if (E->ctx->heredocs && E->ctx->heredoc_count > 0)
		heredoc_release(E->ctx->heredocs, E->ctx->heredoc_count);
	st = run_builtin_parent(E, argv);
	ft_free_env_copy(E->ctx->envp);
	pool_dispose(E->pool);
	exit(st);
}
