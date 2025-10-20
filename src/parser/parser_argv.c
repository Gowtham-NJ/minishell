/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:45:15 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:37:38 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parse_init.h"
#include <stdlib.h>

int	append_word(t_parser *P, char ***argv, size_t *argc, size_t *cap)
{
	char	*dup;
	char	**tmp;

	if (*argc == *cap)
	{
		*cap *= 2;
		if (*cap == 0)
			*cap = 8;
		tmp = (char **)pool_malloc(&P->pool, (*cap + 1) * sizeof(**argv));
		if (!tmp)
		{
			p_set_error(P, "allocation failed", p_cur(P)->lex);
			return (-1);
		}
		ft_copy_argv(*argv, &tmp, *argc);
		*argv = tmp;
	}
	dup = p_sndup(P, p_cur(P)->lex, p_cur(P)->len);
	if (!dup)
		return (p_set_error(P, "allocation failed", p_cur(P)->lex), -1);
	(*argv)[*argc] = dup;
	(*argc)++;
	P->i++;
	return (0);
}

void	free_argv_temp(char **argv, size_t count)
{
	(void)argv;
	(void)count;
}

void	init_simple_parts(t_simple_parts *p)
{
	p->argv = NULL;
	p->argc = 0;
	p->cap = 0;
	p->redirs = NULL;
	p->tail = &p->redirs;
}

int	collect_fail(t_simple_parts *p)
{
	(void)p;
	return (-1);
}

int	parse_simple_command_helper(t_parser *P, t_simple_parts *parts)
{
	t_token	*tk;
	t_redir	*r;

	init_simple_parts(parts);
	while (1)
	{
		tk = p_cur(P);
		if (tk->kind == T_WORD)
		{
			if (append_word(P, &parts->argv, &parts->argc, &parts->cap) < 0)
				return (collect_fail(parts));
			continue ;
		}
		if (is_redir_token(tk->kind))
		{
			r = parse_redirection(P);
			if (!r)
				return (collect_fail(parts));
			*(parts->tail) = r;
			parts->tail = &r->next;
			continue ;
		}
		break ;
	}
	return (0);
}
