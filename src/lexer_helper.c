/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 19:27:41 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 14:58:49 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft/libft.h"
#include "tok.h"
#include <stdlib.h>

/* 1-char operators: | ( ) < >   — NO ';' and NO '&' */
int	read_op1(t_tokvec *tv, t_token *t, char **pp)
{
	char	*p;

	p = *pp;
	if (*p == '|')
		t->kind = T_PIPE;
	else if (*p == '(')
		t->kind = T_LPAR;
	else if (*p == ')')
		t->kind = T_RPAR;
	else if (*p == '<')
		t->kind = T_LT;
	else if (*p == '>')
		t->kind = T_GT;
	else
		return (0);
	t->lex = p;
	t->len = 1;
	*pp = p + 1;
	tv_push(tv, *t);
	return (1);
}

/* 2-char operators: ONLY '<<' and '>>' — NO '&&' and NO '||' */
int	read_op2(t_tokvec *tv, t_token *t, char **pp)
{
	char	*p;

	p = *pp;
	if (p[0] == '&' && p[1] == '&')
		t->kind = T_AND;
	else if (p[0] == '|' && p[1] == '|')
		t->kind = T_OR;
	else if (p[0] == '>' && p[1] == '>')
		t->kind = T_DGT;
	else if (p[0] == '<' && p[1] == '<')
		t->kind = T_HDOC;
	else
		return (read_op1(tv, t, pp));
	t->lex = p;
	t->len = 2;
	*pp = p + 2;
	tv_push(tv, *t);
	return (1);
}

void	tv_push(t_tokvec *tv, t_token t)
{
	t_token	*nv;
	size_t	newcap;
	size_t	i;

	if (tv->n == tv->cap)
	{
		if (tv->cap == 0)
			newcap = 16;
		else
			newcap = tv->cap * 2;
		nv = (t_token *)pool_malloc(tv->pool, newcap * sizeof(*nv));
		if (!nv)
			return ;
		i = 0;
		while (i < tv->n)
		{
			nv[i] = tv->v[i];
			i += 1;
		}
		tv->v = nv;
		tv->cap = newcap;
	}
	tv->v[tv->n] = t;
	tv->n += 1;
}

void	tok_free(t_tokvec *tv)
{
	ft_memset(tv, 0, sizeof * tv);
}
