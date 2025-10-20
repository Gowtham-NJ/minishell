/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_collect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:31:33 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:48:16 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft/libft.h"
#include <dirent.h>
#include <stdlib.h>

static int	vec_grow(t_vecgg *v)
{
	char	**nv;
	size_t	i;
	size_t	nc;

	if (v->c == 0)
		nc = 8;
	else
		nc = v->c * 2;
	nv = (char **)pool_malloc(v->pool, nc * sizeof(char *));
	if (!nv)
		return (-1);
	i = 0;
	while (i < v->n)
	{
		nv[i] = v->d[i];
		i++;
	}
	v->d = nv;
	v->c = nc;
	return (0);
}

static int	push_entry(t_vecgg *v, const char *base, const char *name, DIR *dp)
{
	char	*full;

	full = ft_path_join_pool(v->pool, base, name);
	if (!full)
	{
		closedir(dp);
		return (-1);
	}
	if (v->n == v->c)
	{
		if (vec_grow(v) < 0)
		{
			closedir(dp);
			return (-1);
		}
	}
	v->d[v->n] = full;
	v->n += 1;
	return (0);
}

static int	read_dir_and_fill(const char *dir, const char *pat, t_vecgg *v)
{
	DIR				*dp;
	struct dirent	*e;
	const char		*base;

	if (dir && *dir)
		base = dir;
	else
		base = ".";
	dp = opendir(base);
	if (!dp)
		return (0);
	e = readdir(dp);
	while (e)
	{
		if (!match_entry(pat, e->d_name))
		{
			e = readdir(dp);
			continue ;
		}
		if (push_entry(v, base, e->d_name, dp) < 0)
			return (-1);
		e = readdir(dp);
	}
	closedir(dp);
	return (0);
}

ssize_t	collect_matches(t_pool *pool, const char *dir, const char *pat,
		char ***out)
{
	int		r;
	t_vecgg	v;

	*out = NULL;
	v.d = NULL;
	v.n = 0;
	v.c = 0;
	v.pool = pool;
	r = read_dir_and_fill(dir, pat, &v);
	if (r < 0)
		return (-1);
	if (v.n == 0)
		return (0);
	*out = v.d;
	return ((ssize_t)v.n);
}
