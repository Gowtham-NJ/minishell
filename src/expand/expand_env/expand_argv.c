/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:35:38 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:20:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"

/* takes ownership of expd; pushes all split pieces */
static int	ab_push_split(t_fb *b, char *expd, unsigned char flag)
{
	char	**pieces;
	size_t	k;

	pieces = ifs_split_basic(b->pool, expd);
	(void)expd;
	if (!pieces)
		return (-1);
	k = 0;
	while (pieces[k])
	{
		if (ab_push_word(b, pieces[k], flag) != 0)
		{
			free_vec(pieces, k);
			return (-1);
		}
		k++;
	}
	return (0);
}

/* signature now 3 args: (orig, builder, envp) */
static int	process_arg(t_exec *exec_ctx, const char *orig, t_fb *b,
		unsigned char flag)
{
	char			*expd;
	int				q;
	unsigned char	f;

	expd = expand_word_env(exec_ctx, orig, *exec_ctx->envp, &q);
	if (!expd)
		return (-1);
	f = flag;
	if (!q && contains_glob(expd))
		f = 1;
	if (!q)
	{
		if (expd[0] == '\0')
			return (0);
		return (ab_push_split(b, expd, f));
	}
	if (ab_push_word(b, expd, f) != 0)
		return (-1);
	return (0);
}

/* -- public -------------------------------------------------------------- */

/* tiny helper: replace the ternary without changing behavior */
static unsigned char	get_glob_flag(t_cmd *cmd, size_t i)
{
	unsigned char	g;

	g = 0;
	if (cmd->glob_ok)
		g = cmd->glob_ok[i];
	return (g);
}

/* was: static int collect_args_into_fb(t_exec *exec_ctx, t_cmd *cmd,
**                                      char **envp, t_fb *b) */
static int	collect_args_into_fb(t_exec *exec_ctx, t_cmd *cmd, t_fb *b)
{
	char	**argv;
	size_t	i;

	b->cap = 8;
	b->n = 0;
	b->pool = exec_ctx->pool;
	b->neo = (char **)pool_malloc(b->pool, b->cap * sizeof(char *));
	b->fneo = (unsigned char *)pool_malloc(b->pool, b->cap
			* sizeof(unsigned char));
	if (!b->neo || !b->fneo)
		return (-1);
	b->neo[0] = NULL;
	argv = cmd->argv;
	i = 0;
	while (argv && argv[i])
	{
		if (process_arg(exec_ctx, argv[i], b, get_glob_flag(cmd, i)) != 0)
			return (-1);
		i++;
	}
	cmd->argv = b->neo;
	return (0);
}

/* public entry: keep the exact post-swap glob_ok rebuild */
int	expand_env_in_cmd(t_exec *exec_ctx, t_cmd *cmd)
{
	t_fb	b;
	char	**envp;

	envp = NULL;
	if (exec_ctx && exec_ctx->envp)
		envp = *exec_ctx->envp;
	if (collect_args_into_fb(exec_ctx, cmd, &b) != 0)
		return (-1);
	cmd->glob_ok = (unsigned char *)pool_malloc(exec_ctx->pool, b.n
			* sizeof(unsigned char));
	if (!cmd->glob_ok)
	{
		return (-1);
	}
	ft_memcpy(cmd->glob_ok, b.fneo, b.n * sizeof(unsigned char));
	return (0);
}
