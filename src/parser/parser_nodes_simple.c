/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_nodes_simple.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:38:32 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:36:24 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_init.h"

/* is the current token a control/separator token? */
static int	is_sep_token(t_tok_kind k)
{
	if (k == T_OR || k == T_AND || k == T_SEMI)
		return (1);
	if (k == T_AMP || k == T_PIPE || k == T_RPAR)
		return (1);
	return (0);
}

/* ensure parts->argv is valid when argc==0;
set proper errors; no comma operator */
static int	handle_argc_zero(t_parser *P, t_simple_parts *parts)
{
	t_tok_kind	k;

	if (parts->argc != 0)
	{
		parts->argv[parts->argc] = NULL;
		return (0);
	}
	if (!parts->redirs)
	{
		k = p_cur(P)->kind;
		if (is_sep_token(k))
			p_set_error(P, "unexpected token", p_cur(P)->lex);
		else
			p_set_error(P, "expected command", p_cur(P)->lex);
		return (-1);
	}
	parts->argv = (char **)pool_malloc(&P->pool, sizeof(char *));
	if (!parts->argv)
	{
		return (-1);
	}
	parts->argv[0] = NULL;
	return (0);
}

t_ast	*parse_simple_command(t_parser *P)
{
	t_ast			*node;
	t_simple_parts	parts;

	if (parse_simple_command_helper(P, &parts) < 0)
		return (NULL);
	if (handle_argc_zero(P, &parts) < 0)
		return (NULL);
	node = ast_cmd(P->ast_pool, parts.argv);
	if (!node)
	{
		if (parts.redirs)
			free_redirs_temp(parts.redirs);
		return (NULL);
	}
	if (parts.redirs)
		node->cmd->redirs = parts.redirs;
	return (node);
}
