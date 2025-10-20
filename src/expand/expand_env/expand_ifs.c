/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ifs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:30:42 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:18:45 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"
#include <stdlib.h>

void	free_vec(char **v, size_t start_idx)
{
	(void)v;
	(void)start_idx;
}

/* returns: 1 = got word (sets *w_out), 0 = end, -1 = alloc error */
static int	grab_word(t_pool *pool, const char *s, size_t *i, char **w_out)
{
	size_t	start;

	while (s[*i] == ' ' || s[*i] == '\t' || s[*i] == '\n')
		(*i)++;
	if (!s[*i])
		return (0);
	start = *i;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\n')
		(*i)++;
	*w_out = pool_strndup(pool, s + start, *i - start);
	if (!*w_out)
		return (-1);
	return (1);
}

/* takes ownership of w; grows split->out if needed; NULL-terminates */
static int	append_word(t_pool *pool, t_ifs_split *sp, char *w)
{
	char	**tmp;
	size_t	k;

	if (sp->n + 2 > sp->cap)
	{
		tmp = (char **)pool_malloc(pool, sp->cap * 2 * sizeof(char *));
		if (!tmp)
			return (-1);
		k = 0;
		while (k < sp->n)
		{
			tmp[k] = sp->out[k];
			k++;
		}
		sp->out = tmp;
		sp->cap *= 2;
	}
	sp->out[sp->n++] = w;
	sp->out[sp->n] = NULL;
	return (0);
}

char	**ifs_split_basic(t_pool *pool, const char *s)
{
	t_ifs_split	split;
	char		*w;
	int			r;

	split.s = s;
	split.i = 0;
	split.n = 0;
	split.cap = 4;
	split.out = (char **)pool_malloc(pool, split.cap * sizeof(char *));
	if (!split.out)
		return (NULL);
	split.out[0] = NULL;
	while (1)
	{
		r = grab_word(pool, split.s, &split.i, &w);
		if (r == 0)
			break ;
		if (r < 0 || append_word(pool, &split, w) < 0)
			return (NULL);
	}
	return (split.out);
}
