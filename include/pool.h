/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:59:27 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 09:41:18 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "libft/libft.h"
#include <stddef.h>

typedef struct s_pool_node
{
	void				*ptr;
	struct s_pool_node	*next;
}						t_pool_node;
/* LIFO list of tracked allocations */
typedef struct s_pool
{
	t_pool_node			*head;
	size_t				count;
}						t_pool;

/* lifecycle */
void					pool_init(t_pool *p);
/* frees every tracked pointer + nodes */
void					pool_dispose(t_pool *p);
/* take ownership of an already-malloc'd pointer (e.g., readline result) */
int						pool_track(t_pool *p, void *ptr);
/* drop-in replacements backed by libft where possible */
/* uses malloc */
void					*pool_malloc(t_pool *p, size_t bytes);
/* uses ft_calloc */
void					*pool_calloc(t_pool *p, size_t count, size_t size);
/* uses ft_strdup */
char					*pool_strdup(t_pool *p, const char *s);
/* uses pool_malloc + ft_memcpy */
char					*pool_strndup(t_pool *p, const char *s, size_t n);
/* grow array without realloc: alloc new + copy (old remains tracked) */
void					*pool_realloc_clone(t_pool *p, const void *old,
							size_t old_bytes, size_t new_bytes);
int						pool_push_node(t_pool *p, void *ptr);
