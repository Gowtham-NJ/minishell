/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_chars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:23:40 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/03 18:23:41 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	is_glob_char(int c)
{
	if (c == '*')
		return (1);
	if (c == '?')
		return (1);
	if (c == '[')
		return (1);
	if (c == ']')
		return (1);
	return (0);
}

int	is_quoted_word(const char *s)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		s++;
	}
	return (0);
}

int	contains_glob(const char *s)
{
	while (s && *s)
	{
		if (is_glob_char((unsigned char)*s))
			return (1);
		s++;
	}
	return (0);
}
