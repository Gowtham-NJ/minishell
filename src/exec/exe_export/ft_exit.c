/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 01:11:09 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/19 20:01:33 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "ft_exit.h"

static int	parse_numeric_argument(const char *arg, long long *value)
{
	size_t	i;
	size_t	digits;

	if (!arg)
		return (-1);
	i = 0;
	digits = 0;
	while (arg[i] && ft_isspace(arg[i]))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (ft_isdigit((unsigned char)arg[i]))
	{
		digits++;
		i++;
	}
	while (arg[i] && ft_isspace(arg[i]))
		i++;
	if (digits == 0 || arg[i] != '\0')
		return (-1);
	if (ft_atoi_long_long_check(arg).flag == NOT_OK)
		return (-1);
	*value = ft_atoi_long_long(arg);
	return (0);
}

/* helper: prints "exit" if interactive, validates args, computes code
   returns:
		0  -> ok; *out_code set; caller should terminate (exit/return)
		1  -> "too many arguments" case; shell must NOT exit
	-1  -> invalid inputs (should not happen in normal calls) */
static int	exit_prepare(t_exec *E, char **argv, int in_child,
		unsigned char *out_code)
{
	long long	value;
	const char	*arg;

	if (!in_child && isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!argv[1])
	{
		*out_code = (unsigned char)status_get(E);
		return (0);
	}
	arg = argv[1];
	if (parse_numeric_argument(arg, &value) != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit:%s: numeric argument required\n", arg);
		*out_code = 2;
		return (0);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	*out_code = (unsigned char)value;
	return (0);
}

/* disposing child pool before returning to caller that will _exit */
/* close any fds held in AST before disposing pool */
/* helper: parent-side cleanup + _exit_ */
static void	exit_parent(t_exec *e, unsigned char code)
{
	free(e->line);
	e->line = NULL;
	if (e->ctx)
	{
		free(e->ctx->prompt);
		e->ctx->prompt = NULL;
	}
	fd_save_end(e->sv);
	executor_free_ast(e->ast);
	e->ast = NULL;
	if (e->pool)
	{
		pool_dispose(e->pool);
		e->pool = NULL;
	}
	ctx_end(e->ctx);
	exit(code);
}

int	ft_exit_builtin(t_exec *exec_ctx, char **argv, int in_child)
{
	int				rc;
	unsigned char	code;

	if (!exec_ctx || !argv || !argv[0])
		return (0);
	rc = exit_prepare(exec_ctx, argv, in_child, &code);
	if (rc != 0)
		return (rc == 1);
	if (in_child)
	{
		if (exec_ctx->ctx->heredocs && exec_ctx->ctx->heredoc_count > 0)
			heredoc_release(exec_ctx->ctx->heredocs,
				exec_ctx->ctx->heredoc_count);
		ft_free_env_copy(exec_ctx->ctx->envp);
		pool_dispose(exec_ctx->pool);
		return (code);
	}
	exit_parent(exec_ctx, code);
	return (0);
}
