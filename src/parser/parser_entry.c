/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entry.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:41:35 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 14:03:00 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parse_init.h"

/* worker: does lex → parse → checks; no pool init/dispose here */
static t_ast	*parse_line_to_ast_core(t_parser *P, const char *line)
{
	t_ast	*root;

	if (lex_line(&P->tv, line) != 0)
	{
		P->err_msg = P->tv.err_msg;
		P->err_tok = P->tv.err_tok;
		return (NULL);
	}
	P->i = 0;
	P->err_msg = NULL;
	P->err_tok = NULL;
	root = parse_list(P);
	if (!root)
		return (NULL);
	while (p_accept(P, T_SEMI) || p_accept(P, T_AMP))
		;
	if (p_cur(P)->kind != T_END)
	{
		p_set_error(P, "unexpected t_token", p_cur(P)->lex);
		return (NULL);
	}
	return (root);
}

/* arena for temporary parse allocations */
/* connect token vector to parser pool */
/* AST allocated from exec pool; will be disposed there */
/* dispose parser pool before returning AST (AST is exec-pool-owned) */
t_ast	*parse_line_to_ast(t_parser *P, const char *line)
{
	t_ast	*root;
	char	*tok_dup;

	tok_dup = NULL;
	pool_init(&P->pool);
	P->tv.pool = &P->pool;
	P->err_tok_mem = NULL;
	root = parse_line_to_ast_core(P, line);
	if (!root && P->err_tok)
		tok_dup = ft_strdup((char *)P->err_tok);
	pool_dispose(&P->pool);
	if (tok_dup)
	{
		P->err_tok_mem = tok_dup;
		P->err_tok = P->err_tok_mem;
	}
	else
	{
		P->err_tok_mem = NULL;
		if (!root)
			P->err_tok = NULL;
	}
	return (root);
}

t_ast	*parse_and_or(t_parser *P)
{
	t_ast		*left;
	t_tok_kind	op;
	t_ast		*right;

	left = parse_pipeline(P);
	if (!left)
		return (NULL);
	while (p_cur(P)->kind == T_AND || p_cur(P)->kind == T_OR)
	{
		op = p_cur(P)->kind;
		P->i++;
		right = parse_pipeline(P);
		if (!right)
		{
			p_set_error(P, "expected command", p_cur(P)->lex);
			return (NULL);
		}
		if (op == T_AND)
			left = ast_and(P->ast_pool, left, right);
		else
			left = ast_or(P->ast_pool, left, right);
	}
	return (left);
}

t_ast	*parse_list(t_parser *P)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_and_or(P);
	if (!left)
		return (NULL);
	while (p_accept(P, T_SEMI) || p_accept(P, T_AMP))
	{
		if (p_cur(P)->kind == T_END || p_cur(P)->kind == T_RPAR)
			break ;
		right = parse_and_or(P);
		if (!right)
		{
			p_set_error(P, "expected command", p_cur(P)->lex);
			return (NULL);
		}
		left = ast_list(P->ast_pool, left, right);
	}
	return (left);
}
