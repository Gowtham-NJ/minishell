/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:36:56 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/04 14:02:23 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_init.h"
#include <stdlib.h>

int	is_redir_token(t_tok_kind k)
{
	return (k == T_LT || k == T_GT || k == T_DGT || k == T_HDOC);
}

char	*parse_redir_target(t_parser *P)
{
	char	*target;

	if (p_cur(P)->kind != T_WORD)
	{
		p_set_error(P, "expected filename", p_cur(P)->lex);
		return (NULL);
	}
	target = p_sndup(P, p_cur(P)->lex, p_cur(P)->len);
	if (!target)
	{
		p_set_error(P, "allocation failed", p_cur(P)->lex);
		return (NULL);
	}
	P->i++;
	return (target);
}

t_redir	*redir_from_kind(t_pool *pool, t_tok_kind kind, const char *target)
{
	if (kind == T_LT)
		return (redir_new(pool, R_IN, target, 0));
	if (kind == T_GT)
		return (redir_new(pool, R_OUT, target, 1));
	if (kind == T_DGT)
		return (redir_new(pool, R_APP, target, 1));
	return (redir_new(pool, R_HDOC, target, 0));
}

t_redir	*parse_redirection(t_parser *P)
{
	t_tok_kind	kind;
	char		*target;
	t_redir		*redir;

	kind = p_cur(P)->kind;
	P->i++;
	target = parse_redir_target(P);
	if (!target)
		return (NULL);
	redir = redir_from_kind(P->ast_pool, kind, target);
	return (redir);
}

void	free_redirs_temp(t_redir *r)
{
	(void)r;
}
