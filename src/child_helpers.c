/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell                                    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:00:00 by minishell         #+#    #+#             */
/*   Updated: 2025/10/16 00:00:00 by minishell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "child_helpers.h"
#include <unistd.h>

size_t	vec_len(char *const *v)
{
	size_t	n;

	if (!v)
		return (0);
	n = 0;
	while (v[n])
		n++;
	return (n);
}

char	**vec_append_dup(char *const *v, const char *add, t_pool *pool)
{
	size_t	n;
	char	**neo;

	n = vec_len(v);
	neo = (char **)pool_malloc(pool, (n + 2) * sizeof(*neo));
	if (!neo)
		return (NULL);
	if (n)
		ft_memcpy(neo, v, n * sizeof(*neo));
	if (add)
		neo[n] = pool_strdup(pool, add);
	else
		neo[n] = NULL;
	if (!neo[n])
		return (NULL);
	neo[n + 1] = NULL;
	return (neo);
}

int	fd_set_cloexec(int fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFD);
	if (flags == -1)
		return (-1);
	if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
		return (-1);
	return (0);
}
