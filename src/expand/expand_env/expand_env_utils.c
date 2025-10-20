/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 10:31:32 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:01:54 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"

/* helpers copied to handle redir globbing */
int	build_dir_from_span(t_pool *pool, const char *w, char **dir, size_t len)
{
	size_t	i;

	*dir = NULL;
	i = 0;
	while (i < len)
	{
		if (is_glob_char((unsigned char)w[i]))
			return (1);
		i++;
	}
	*dir = (char *)pool_malloc(pool, len + 1);
	if (!*dir)
		return (-1);
	ft_memcpy(*dir, w, len);
	(*dir)[len] = '\0';
	return (0);
}

int	build_dir_from_word(t_pool *pool, const char *w, char **dir,
		const char **pat)
{
	char	*s;
	size_t	len;

	*dir = NULL;
	*pat = w;
	s = (char *)ft_strrchr(w, '/');
	if (!s)
	{
		*dir = pool_strdup(pool, ".");
		if (!*dir)
			return (-1);
		return (0);
	}
	len = (size_t)(s - w);
	*pat = s + 1;
	if (len == 0 && w[0] == '/')
	{
		*dir = pool_strdup(pool, "/");
		if (!*dir)
			return (-1);
		return (0);
	}
	return (build_dir_from_span(pool, w, dir, len));
}

/* ---------- helpers for apply_glob_redir ---------------- */
int	redir_dir_pat(t_pool *pool, const char *t, char **dir, const char **pat)
{
	int	r;

	*dir = NULL;
	*pat = NULL;
	r = build_dir_from_word(pool, t, dir, pat);
	if (r < 0)
		return (-1);
	if (r != 0 || **pat == '\0')
	{
		(void)dir;
		return (0);
	}
	return (1);
}

static int	ab_grow(t_fb *b)
{
	char			**tmp;
	unsigned char	*ftmp;
	size_t			k;

	if (b->n + 2 <= b->cap)
		return (0);
	tmp = (char **)pool_malloc(b->pool, (b->cap * 2) * sizeof(char *));
	ftmp = (unsigned char *)pool_malloc(b->pool, (b->cap * 2)
			* sizeof(unsigned char));
	if (!tmp || !ftmp)
		return (-1);
	k = 0;
	while (k < b->n)
	{
		tmp[k] = b->neo[k];
		ftmp[k] = b->fneo[k];
		k++;
	}
	b->neo = tmp;
	b->fneo = ftmp;
	b->cap = b->cap * 2;
	return (0);
}

/* takes ownership of w */
int	ab_push_word(t_fb *b, char *w, unsigned char flag)
{
	if (!w)
		return (-1);
	if (ab_grow(b) != 0)
	{
		return (-1);
	}
	b->neo[b->n] = w;
	b->fneo[b->n] = flag;
	b->n = b->n + 1;
	b->neo[b->n] = NULL;
	return (0);
}
