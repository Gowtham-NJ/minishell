/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:23:21 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 10:20:42 by vcerny           ###   ########.fr       */
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
#include <stdlib.h>
#include <stdio.h>

int	pool_push_node(t_pool *p, void *ptr)
{
	t_pool_node	*node;

	if (!p)
		return (0);
	node = (t_pool_node *)malloc(sizeof(*node));
	if (!node)
		return (-1);
	node->ptr = ptr;
	node->next = p->head;
	p->head = node;
	p->count++;
	return (0);
}

char	*pool_strndup(t_pool *p, const char *s, size_t n)
{
	char	*q;

	if (p)
		q = (char *)pool_malloc(p, n + 1);
	else
		q = (char *)malloc(n + 1);
	if (!q)
		return (NULL);
	if (n && s)
		ft_memcpy(q, s, n);
	q[n] = '\0';
	return (q);
}

void	*pool_realloc_clone(t_pool *p, const void *old, size_t old_bytes,
		size_t new_bytes)
{
	void	*neo;
	size_t	c;

	if (p)
		neo = pool_malloc(p, new_bytes);
	else
		neo = malloc(new_bytes);
	if (!neo)
		return (NULL);
	if (old && old_bytes)
	{
		if (old_bytes < new_bytes)
			c = old_bytes;
		else
			c = new_bytes;
		ft_memcpy(neo, old, c);
	}
	return (neo);
}

void	pool_dispose(t_pool *p)
{
	t_pool_node	*cur;
	t_pool_node	*next;

	if (!p)
		return ;
	cur = p->head;
	while (cur)
	{
		next = cur->next;
		free(cur->ptr);
		free(cur);
		cur = next;
	}
	p->head = NULL;
	p->count = 0;
}

void	pool_init(t_pool *p)
{
	p->head = NULL;
	p->count = 0;
}
