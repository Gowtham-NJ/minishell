/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:22:56 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/04 13:30:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft/libft.h"
#include <stdlib.h>

char	*ft_path_join_pool(t_pool *pool, const char *dir, const char *name)
{
	size_t	dlen;
	size_t	nlen;
	char	*out;

	if (dir[0] == '.' && dir[1] == '\0')
		return (pool_strdup(pool, (char *)name));
	dlen = ft_strlen(dir);
	nlen = ft_strlen(name);
	out = (char *)pool_malloc(pool, dlen + 1 + nlen + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, dir, dlen);
	out[dlen] = '/';
	ft_memcpy(out + dlen + 1, name, nlen + 1);
	return (out);
}

size_t	argv_count(char **argv)
{
	size_t	n;

	n = 0;
	while (argv && argv[n])
		n++;
	return (n);
}

int	strptr_insertion_sort(char **v, size_t n)
{
	size_t	i;
	size_t	j;
	char	*key;

	i = 1;
	while (i < n)
	{
		key = v[i];
		j = i;
		while (j > 0 && ft_strcmp(v[j - 1], key) > 0)
		{
			v[j] = v[j - 1];
			j--;
		}
		v[j] = key;
		i++;
	}
	return (0);
}
