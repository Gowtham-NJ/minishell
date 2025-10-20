/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:33 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 22:43:49 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "proof.h"
#include "tok.h"

typedef struct s_parser
{
	t_tokvec	tv;
	size_t		i;
	const char	*err_msg;
	const char	*err_tok;
	char		*err_tok_mem;
	/* temporary allocations during parse */
	t_pool		pool;
	/* AST allocations should go to this external pool (exec_ctx->pool) */
	t_pool		*ast_pool;
}				t_parser;

t_ast			*parse_line_to_ast(t_parser *P, const char *line);
