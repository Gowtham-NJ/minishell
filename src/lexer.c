/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:38:47 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 14:00:45 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer_utils.h"
#include "libft/libft.h"
#include "tok.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* advance *pp to just after the matching ')'; 0 on success, -1 on EOF */
static int	skip_cmd_subst(char **pp)
{
	char	*p;
	int		r;
	int		depth;
	int		q;

	p = *pp + 2;
	depth = 1;
	q = 0;
	while (*p)
	{
		r = try_special_cmdsubst(&p, &depth, q);
		if (r)
		{
			if (r == 2 && depth == 0)
			{
				*pp = p;
				return (0);
			}
			continue ;
		}
		update_quote(*p, &q);
		p++;
	}
	return (-1);
}

/* set lexer error and return -1 */
static int	set_lex_err(t_tokvec *tv, t_token *t, const char *msg)
{
	tv->err_msg = msg;
	tv->err_tok = t->lex;
	return (-1);
}

/* read a WORD token (quote-aware); 0 ok, -1 on error */
static int	read_word(t_tokvec *tv, t_token *t, char **pp)
{
	char	*p;

	p = *pp;
	t->kind = T_WORD;
	t->lex = p;
	while (!is_word_delim(*p))
	{
		if (*p == '\'' || *p == '"')
		{
			if (skip_quoted(&p) < 0)
				return (set_lex_err(tv, t, "unclosed quote"));
			continue ;
		}
		if (*p == '$' && p[1] == '(')
		{
			if (skip_cmd_subst(&p) < 0)
				return (set_lex_err(tv, t,
						"command substitution: expected ')'"));
			continue ;
		}
		p++;
	}
	t->len = (int)(p - t->lex);
	*pp = p;
	return (0);
}

/* helper: get next t_token; 1=ok/pushed, 0=eol, -1=error */
static int	next_token(t_tokvec *tv, char **pp)
{
	char	*p;
	t_token	t;

	p = *pp;
	while (*p && isspace((unsigned char)*p))
		p++;
	if (!*p || read_op2(tv, &t, &p))
	{
		*pp = p;
		if (!*p)
			return (0);
		return (1);
	}
	if (read_word(tv, &t, &p) < 0)
		return (-1);
	tv_push(tv, t);
	*pp = p;
	return (1);
}

int	lex_line(t_tokvec *tv, const char *line)
{
	char	*p;
	int		st;
	t_token	t;
	t_pool	*saved_pool;

	saved_pool = tv->pool;
	ft_memset(tv, 0, sizeof(*tv));
	tv->pool = saved_pool;
	tv->buf = pool_strdup(tv->pool, (char *)line);
	if (!tv->buf)
		return (-1);
	p = tv->buf;
	while (1)
	{
		st = next_token(tv, &p);
		if (st <= 0)
			break ;
	}
	if (st < 0)
		return (-1);
	t.kind = T_END;
	t.lex = NULL;
	t.len = 0;
	tv_push(tv, t);
	return (0);
}
