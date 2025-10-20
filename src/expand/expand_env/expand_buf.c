/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:35:16 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:18:55 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	buf_init(t_buf *b, t_pool *pool, size_t cap)
{
	b->pool = pool;
	b->data = (char *)pool_malloc(pool, cap);
	if (!b->data)
		return (-1);
	b->len = 0;
	b->cap = cap;
	return (0);
}

int	buf_grow(t_buf *b, size_t need)
{
	char	*newd;
	size_t	ncap;

	ncap = b->cap;
	while (b->len + need + 1 > ncap)
		ncap *= 2;
	newd = (char *)pool_malloc(b->pool, ncap);
	if (!newd)
		return (-1);
	ft_memcpy(newd, b->data, b->len);
	b->data = newd;
	b->cap = ncap;
	return (0);
}

int	buf_putc(t_buf *b, char c)
{
	if (b->len + 2 > b->cap && buf_grow(b, 1) != 0)
		return (-1);
	b->data[b->len] = c;
	b->len += 1;
	return (0);
}

int	buf_putn(t_buf *b, const char *s, size_t n)
{
	if (b->len + n + 1 > b->cap && buf_grow(b, n) != 0)
		return (-1);
	ft_memcpy(b->data + b->len, s, n);
	b->len += n;
	return (0);
}

char	*buf_take(t_buf *b)
{
	char	*res;

	if (b->data)
		b->data[b->len] = '\0';
	res = b->data;
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
	return (res);
}
