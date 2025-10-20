/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:23:21 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 09:40:39 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
README (pool usage & patterns)
--------------------------------
Use t_pool to group related allocations so they can be freed in one call:

- Parser phase (per command): put a t_pool in t_parser for all temporary
  allocations while parsing. Dispose when the AST is finalized or discarded.

- Executor / shell lifetime: put a t_pool in t_exec for data tied to a single
  evaluation/execution cycle. Keep shell-global state in a higher-scope pool
  if needed, but prefer clear lifetimes.

- Child after fork(): create a fresh child-local t_pool immediately after
  fork() in the child. Duplicate only what is needed for exec/builtin. On
  success execve() replaces the address space; on failure or for builtins,
  pool_dispose(&child) and _exit(status). Prefer _exit over exit in children
  to avoid running atexit handlers and flushing shared stdio twice.

Replace direct malloc/ft_calloc/ft_strdup/strndup with pool_* variants to
ensure allocations are tracked and released:
  - pool_malloc / pool_calloc / pool_strdup / pool_strndup

Dynamic array growth (no realloc):
  - Allocate a new buffer with pool_malloc and ft_memcpy the used bytes.
  - Switch to the new pointer; old memory stays tracked and will be freed by
	pool_dispose. This avoids realloc while keeping code simple.
*/
#include "pool.h"
#include <stdlib.h> /* malloc, free */

// static int	pool_push_node(t_pool *p, void *ptr)
// {
// 	t_pool_node	*node;

// 	if (!p)
// 		return (0);
// 	node = (t_pool_node *)malloc(sizeof(*node));
// 	if (!node)
// 		return (-1);
// 	node->ptr = ptr;
// 	node->next = p->head;
// 	p->head = node;
// 	p->count++;
// 	return (0);
// }

int	pool_track(t_pool *p, void *ptr)
{
	if (!ptr)
		return (0);
	return (pool_push_node(p, ptr));
}

void	*pool_malloc(t_pool *p, size_t bytes)
{
	void	*q;

	q = malloc(bytes);
	if (!q)
		return (NULL);
	if (pool_push_node(p, q) < 0)
	{
		free(q);
		return (NULL);
	}
	return (q);
}

void	*pool_calloc(t_pool *p, size_t count, size_t size)
{
	void	*q;

	q = ft_calloc(count, size);
	if (!q)
		return (NULL);
	if (pool_push_node(p, q) < 0)
	{
		free(q);
		return (NULL);
	}
	return (q);
}

char	*pool_strdup(t_pool *p, const char *s)
{
	char	*q;

	q = ft_strdup((char *)s);
	if (!q)
		return (NULL);
	if (pool_push_node(p, q) < 0)
	{
		free(q);
		return (NULL);
	}
	return (q);
}
