/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:41 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:47:49 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "exec.h"
#include "executor.h"
#include "expand.h"
#include "ft_export.h"
#include "libft/libft.h"
#include "proof.h"
#include "status.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_vec
{
	char		**d;
	size_t		n;
	size_t		c;
	t_pool		*pool;
}				t_vecgg;

typedef struct s_buf
{
	char		*data;
	size_t		len;
	size_t		cap;
	t_pool		*pool;
}				t_buf;

int				buf_init(t_buf *b, t_pool *pool, size_t cap);
int				buf_grow(t_buf *b, size_t need);
int				buf_putc(t_buf *b, char c);
int				buf_putn(t_buf *b, const char *s, size_t n);
char			*buf_take(t_buf *b);

typedef struct s_status_iter
{
	const char	*word;
	const char	*status;
	size_t		status_len;
	size_t		index;
	size_t		length;
	size_t		write_pos;
	int			state;
}				t_status_iter;

int				status_apply_exit(t_pool *pool, t_ast *node, const char *status,
					size_t status_len);
char			*expand_word_env(t_exec *exec_ctx, const char *word,
					char **envp, int *was_quoted);
int				has_unquoted_glob(const char *s);
int				expand_globs_in_cmd(t_cmd *cmd);
int				expand_ast(t_ast *root);
int				expand_exit_status(t_exec *exec_ctx, t_ast *root);
int				expand_env_vars_in_ast(t_exec *exec_ctx, t_ast *root);
int				expand_env_in_cmd(t_exec *exec_ctx, t_cmd *cmd);
int				is_glob_char(int c);
int				is_quoted_word(const char *s);
int				contains_glob(const char *s);
char			*ft_path_join_pool(t_pool *pool, const char *dir,
					const char *name);
int				pattern_match(const char *pat, const char *str);
int				match_entry(const char *pattern, const char *name);
size_t			argv_count(char **argv);
int				strptr_insertion_sort(char **v, size_t n);
ssize_t			collect_matches(t_pool *pool, const char *dir, const char *pat,
					char ***out);
int				replace_argv_segment(t_cmd *cmd, int idx, char **ms, size_t n);
int				cd_to_oldpwd(char **env);
int				cd_perror(const char *arg);
const char		*env_get(char **env, const char *key);
int				update_quote(int ch, int *q);
