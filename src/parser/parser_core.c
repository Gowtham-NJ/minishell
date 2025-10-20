/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:44:03 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 16:06:47 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "parse_init.h"
#include <stdlib.h>
#include <string.h>

t_token	*p_cur(t_parser *P)
{
	return (&P->tv.v[P->i]);
}

int	p_accept(t_parser *P, t_tok_kind k)
{
	if (p_cur(P)->kind == k)
	{
		P->i++;
		return (1);
	}
	return (0);
}

void	p_set_error(t_parser *P, const char *msg, const char *tok)
{
	const char	*adj;

	if (P->err_msg)
		return ;
	adj = tok;
	if (adj && *adj == '(' && adj > P->tv.buf)
	{
		while (adj > P->tv.buf && adj[-1] == '$')
			adj--;
	}
	if (!P->err_msg)
	{
		P->err_msg = msg;
		P->err_tok = adj;
	}
}

char	*p_sndup(t_parser *P, const char *s, size_t n)
{
	char	*p;

	p = pool_malloc(&P->pool, n + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, s, n);
	p[n] = '\0';
	return (p);
}

void	ft_copy_argv(char **argv, char ***tmp, size_t argc)
{
	size_t	i;

	i = 0;
	while (i < argc)
	{
		(*tmp)[i] = argv[i];
		i++;
	}
}
