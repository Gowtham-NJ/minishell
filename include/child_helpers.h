/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_helpers.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell                                    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:00:00 by minishell         #+#    #+#             */
/*   Updated: 2025/10/16 00:00:00 by minishell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft/libft.h"
#include "pool.h"
#include <fcntl.h>
#include <stddef.h>

size_t				vec_len(char *const *v);

/*
 * Append a new string to a char** vector without realloc.
 * - Allocates a new array of size (n + 2) in the provided pool
 *
	- Copies existing pointers with ft_memcpy
	 (does NOT duplicate existing strings)
 * - Duplicates only the new string into the pool
 * - Ensures the result is NULL-terminated
 * Returns the new vector head (old vector remains valid and tracked elsewhere).
 */
char				**vec_append_dup(char *const *v, const char *add,
						t_pool *pool);

/* Convenience aliases for argv/envp use cases */
static inline char	**argv_append_dup(char *const *argv, const char *add,
		t_pool *p)
{
	return (vec_append_dup(argv, add, p));
}

static inline char	**env_append_dup(char *const *envp, const char *add,
		t_pool *p)
{
	return (vec_append_dup(envp, add, p));
}

/* Set FD_CLOEXEC on a file descriptor; returns 0
on success, -1 on error. */
int					fd_set_cloexec(int fd);
