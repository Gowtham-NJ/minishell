/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:49 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:58:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <termios.h>
#include "pool.h"

typedef struct s_ast		t_ast;
typedef struct s_heredoc	t_heredoc;

typedef struct s_ctx
{
	char					**envp;
	int						lines_so_far;
	char					*prompt;
	int						last_status;
	t_ast					*ast;
	t_heredoc				*heredocs;
	int						heredoc_count;
	struct termios			snap;
	/* shell-lifetime allocation pool */
	t_pool					pool;
}							t_ctx;

char						**ft_make_env_copy(char **envp, size_t wanted_len);
void						ft_free_env_copy(char **env_copy);
size_t						ft_get_env_len(char **envp);

int							ctx_begin(t_ctx *c, char **envp);
void						ctx_reset(t_ctx *c);
void						ctx_end(t_ctx *c);
