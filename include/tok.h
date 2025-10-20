/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:16 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/10 19:51:35 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stddef.h>
#include "pool.h"

typedef enum e_tok_kind
{
	T_WORD,
	T_PIPE,
	T_AND,
	T_OR,
	T_SEMI,
	T_AMP,
	T_LPAR,
	T_RPAR,
	T_LT,
	T_GT,
	T_DGT,
	T_HDOC,
	T_END,
	T_ERR
}				t_tok_kind;

typedef struct s_token
{
	t_tok_kind	kind;
	const char	*lex;
	int			len;
}				t_token;

typedef struct s_tokvec
{
	t_token		*v;
	size_t		n;
	size_t		cap;
	char		*buf;
	const char	*err_msg;
	const char	*err_tok;
	/* allocations tracked via this pool (parser-owned) */
	t_pool		*pool;
}				t_tokvec;

int				lex_line(t_tokvec *tv, const char *line);
void			tok_free(t_tokvec *tv);
void			tv_push(t_tokvec *tv, t_token t);
int				read_op2(t_tokvec *tv, t_token *t, char **pp);
int				read_op1(t_tokvec *tv, t_token *t, char **pp);
