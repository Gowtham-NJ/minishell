/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:54:28 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:59:09 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "expand.h"
#include <stddef.h>

typedef struct s_exp_ctx
{
	t_buf			out;
	const char		*word;
	size_t			i;
	int				quote;
	int				was_quoted;
	char			**envp;
	t_exec			*exec_ctx;
}					t_exp_ctx;
typedef struct s_ifs_split
{
	const char		*s;
	size_t			i;
	size_t			n;
	size_t			cap;
	char			**out;
}					t_ifs_split;
typedef struct s_ab
{
	char			**neo;
	size_t			n;
	size_t			cap;
}					t_ab;

typedef struct s_fb
{
	char			**neo;
	unsigned char	*fneo;
	size_t			n;
	size_t			cap;
	t_pool			*pool;
}					t_fb;

char				**ifs_split_basic(t_pool *pool, const char *s);
void				free_vec(char **v, size_t start_idx);
int					build_dir_from_word(t_pool *pool, const char *w, char **dir,
						const char **pat);
int					build_dir_from_span(t_pool *pool, const char *w, char **dir,
						size_t len);
int					redir_dir_pat(t_pool *pool, const char *t, char **dir,
						const char **pat);
int					ab_push_word(t_fb *b, char *w, unsigned char flag);
int					env_write_take(char *name, char **envp, t_buf *out);
int					cs_span_step(const char *s, size_t *j, int *depth,
						int quote);
char				*command_subst_eval(t_exp_ctx *c, const char *body);
void				command_subst_exec_child(t_exec *exec_ctx, const char *body,
						int write_fd);
