/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Codex                                         +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 20:40:00 by Codex             #+#    #+#             */
/*   Updated: 2025/10/11 20:40:00 by Codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

/* updates quote state and returns 1 iff c is an unquoted '*' or '?' */
static int	step_quote_and_is_unquoted_glob(int *q, char c)
{
	if (*q == 0)
	{
		if (c == '\'' || c == '"')
			*q = (int)c;
		else if (c == '*' || c == '?')
			return (1);
	}
	else if (c == (char)*q)
		*q = 0;
	return (0);
}

int	has_unquoted_glob(const char *s)
{
	int			q;
	int			esc;
	const char	*p;
	char		c;

	q = 0;
	esc = 0;
	p = s;
	while (*p)
	{
		c = *p++;
		if (esc)
		{
			esc = 0;
			continue ;
		}
		if (c == '\\')
		{
			esc = 1;
			continue ;
		}
		if (step_quote_and_is_unquoted_glob(&q, c))
			return (1);
	}
	return (0);
}
