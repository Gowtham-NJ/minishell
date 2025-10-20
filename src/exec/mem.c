/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:24:11 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 16:57:03 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	*sdup(t_pool *pool, const char *s)
{
	size_t	n;
	char	*p;

	if (!s)
		return (NULL);
	n = ft_strlen(s);
	n += 1;
	p = (char *)pool_calloc(pool, n, sizeof(*p));
	if (!p)
	{
		perror("malloc");
		exit(1);
	}
	ft_memcpy(p, s, n);
	return (p);
}

/* duplicate a NULL-terminated argv vector */
char	**dup_argv(t_pool *pool, char **v)
{
	int		n;
	int		i;
	char	**o;

	n = 0;
	while (v && v[n])
		n += 1;
	o = (char **)pool_calloc(pool, n + 1, sizeof(*o));
	if (!o)
	{
		perror("malloc");
		exit(1);
	}
	i = 0;
	while (i < n)
	{
		o[i] = pool_strdup(pool, v[i]);
		i += 1;
	}
	o[n] = NULL;
	return (o);
}
