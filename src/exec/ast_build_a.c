/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build_a.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:16:27 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 12:52:19 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "expand.h"

static unsigned char	*build_glob_flags(t_pool *pool, char **argv)
{
	size_t			argc;
	size_t			i;
	unsigned char	*flags;

	i = 0;
	argc = argv_count(argv);
	if (argc == 0)
		return (NULL);
	flags = (unsigned char *)pool_calloc(pool, argc, sizeof(*flags));
	if (!flags)
	{
		perror("malloc");
		exit(1);
	}
	while (i < argc)
	{
		if (is_assignment_word(argv[i]))
			flags[i] = 0;
		else
			flags[i] = (unsigned char)has_unquoted_glob(argv[i]);
		i++;
	}
	return (flags);
}

/* helper: duplicate a NULL-terminated argv into the pool */
static char	**dup_argv_pool(t_pool *pool, char **argv)
{
	int		n;
	int		i;
	char	**dst;

	n = 0;
	while (argv && argv[n])
		n++;
	dst = (char **)pool_malloc(pool, (size_t)(n + 1) * sizeof(char *));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dst[i] = pool_strdup(pool, argv[i]);
		if (!dst[i])
			return (NULL);
		i++;
	}
	dst[n] = NULL;
	return (dst);
}

t_ast	*ast_cmd(t_pool *pool, char **argv)
{
	t_ast	*n;
	t_cmd	*c;

	n = node(pool, NK_CMD);
	c = (t_cmd *)pool_calloc(pool, 1, sizeof(*c));
	if (!c)
		return (NULL);
	c->pool = pool;
	c->argv = dup_argv_pool(pool, argv);
	if (!c->argv)
		return (NULL);
	c->glob_ok = build_glob_flags(pool, c->argv);
	c->is_builtin = 0;
	if (c->argv && c->argv[0])
		c->is_builtin = is_builtin(c->argv[0]);
	n->cmd = c;
	return (n);
}

t_ast	*ast_pipe(t_pool *pool, t_ast *a, t_ast *b)
{
	t_ast	*n;

	n = node(pool, NK_PIPE);
	n->left = a;
	n->right = b;
	return (n);
}

t_ast	*ast_or(t_pool *pool, t_ast *a, t_ast *b)
{
	t_ast	*n;

	n = node(pool, NK_OR);
	n->left = a;
	n->right = b;
	return (n);
}
