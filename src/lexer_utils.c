/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:38:47 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/14 23:22:08 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer_utils.h"
#include "libft/libft.h"
#include "tok.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* read a WORD t_token (quote-aware); sets error on unclosed quote.
   Returns 0 on success, -1 on error. Does not push into tv. */
/* delimiter for WORD scanning: space or shell operator */
int	is_word_delim(int c)
{
	if (c == 0)
		return (1);
	if (isspace((unsigned char)c))
		return (1);
	if (c == '|' || c == '&' || c == ';')
		return (1);
	if (c == '(' || c == ')' || c == '<' || c == '>')
		return (1);
	return (0);
}

/* advance past a quoted segment; return 0 ok, -1 if unclosed */
int	skip_quoted(char **pp)
{
	char	*p;
	int		q;

	p = *pp;
	q = *p;
	p++;
	while (*p)
	{
		if (*p == q)
		{
			*pp = p + 1;
			return (0);
		}
		p++;
	}
	return (-1);
}

/* handle escapes and parens for $(...) scanning
   returns: 0=none, 1=consumed, 2=closed ')' */
int	try_special_cmdsubst(char **p, int *depth, int q)
{
	char	*s;

	s = *p;
	if (*s == '\\' && q != 1 && s[1])
	{
		*p = s + 2;
		return (1);
	}
	if (q == 0 && *s == '(')
	{
		*p = s + 1;
		*depth += 1;
		return (1);
	}
	if (q == 0 && *s == ')')
	{
		*p = s + 1;
		*depth -= 1;
		return (2);
	}
	return (0);
}
