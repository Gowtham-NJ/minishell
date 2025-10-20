/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_nodes_compound.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:41:20 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:36:34 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_init.h"

int	parse_group_redirs(t_parser *P, t_redir **out_head)
{
	t_redir	*head;
	t_redir	**tail;
	t_redir	*r;

	head = NULL;
	tail = &head;
	while (is_redir_token(p_cur(P)->kind))
	{
		r = parse_redirection(P);
		if (!r)
		{
			free_redirs_temp(head);
			*out_head = NULL;
			return (-1);
		}
		*tail = r;
		tail = &r->next;
	}
	*out_head = head;
	return (0);
}

t_ast	*parse_group(t_parser *P)
{
	t_ast	*inner;
	t_ast	*grp;
	t_redir	*head;

	if (!p_accept(P, T_LPAR))
		return (NULL);
	inner = parse_list(P);
	if (!inner)
		return (NULL);
	if (!p_accept(P, T_RPAR))
	{
		p_set_error(P, "expected ')'", p_cur(P)->lex);
		return (NULL);
	}
	if (parse_group_redirs(P, &head) < 0)
		return (NULL);
	grp = ast_group(P->ast_pool, inner);
	grp->redirs = head;
	return (grp);
}

t_ast	*parse_command(t_parser *P)
{
	if (p_cur(P)->kind == T_LPAR)
		return (parse_group(P));
	return (parse_simple_command(P));
}

t_ast	*parse_pipeline(t_parser *P)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(P);
	if (!left)
		return (NULL);
	while (p_accept(P, T_PIPE))
	{
		right = parse_command(P);
		if (!right)
		{
			p_set_error(P, "expected command", p_cur(P)->lex);
			return (NULL);
		}
		left = ast_pipe(P->ast_pool, left, right);
	}
	return (left);
}
