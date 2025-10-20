/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_pattern.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:22:41 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/14 16:45:34 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static int	brkt_step(const char **pp, char c, int *ok, int *prev)
{
	if (**pp == '-' && *prev != -1 && (*pp)[1] && (*pp)[1] != ']')
	{
		if (c >= *prev && c <= (*pp)[1])
			*ok = 1;
		*pp += 2;
		*prev = -1;
		return (0);
	}
	if (c == **pp)
		*ok = 1;
	*prev = **pp;
	(*pp)++;
	return (0);
}

static int	match_bracket(const char **pp, char c)
{
	int	inv;
	int	ok;
	int	prev;

	inv = 0;
	if (**pp == '!')
	{
		inv = 1;
		(*pp)++;
	}
	ok = 0;
	prev = -1;
	while (**pp && **pp != ']')
	{
		if (brkt_step(pp, c, &ok, &prev) < 0)
			return (-1);
	}
	if (**pp != ']')
		return (-1);
	if ((ok && inv) || (!ok && !inv))
		return (0);
	return (1);
}

static int	pattern_after_star(const char *p, const char *s)
{
	while (*p == '*')
		p++;
	if (*p == '\0')
		return (1);
	while (*s)
	{
		if (pattern_match(p, s))
			return (1);
		s++;
	}
	return (pattern_match(p, s));
}

/* glob matcher: supports '*', '?', and bracket classes with '!' negate */
int	pattern_match(const char *p, const char *s)
{
	int	m;

	if (*p == '\0')
		return (*s == '\0');
	if (*p == '*')
		return (pattern_after_star(p, s));
	if (*s == '\0')
		return (0);
	if (*p == '?')
		return (pattern_match(p + 1, s + 1));
	if (*p == '[')
	{
		p++;
		m = match_bracket(&p, *s);
		if (m != 1)
			return (0);
		return (pattern_match(p + 1, s + 1));
	}
	if (*p == *s)
		return (pattern_match(p + 1, s + 1));
	return (0);
}

/* leading-dot rule: hidden names require pattern starting with '.' */
/* never match "." or ".." via globbing */
int	match_entry(const char *pattern, const char *name)
{
	if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.'
				&& name[2] == '\0')))
		return (0);
	if (name[0] == '.' && pattern[0] != '.')
		return (0);
	return (pattern_match(pattern, name));
}
