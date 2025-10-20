/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_cmd_replace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:42:11 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:24:04 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft/libft.h"
#include <stdlib.h>

static size_t	copy_ptr_block(char **dst, size_t o, char **src, size_t count)
{
	if (count > 0)
	{
		ft_memcpy(dst + o, src, count * sizeof(char *));
		o += count;
	}
	return (o);
}

/* Replace argv[idx] with ms[0..n-1] (or keep argv[idx] if n==0).
   Preserves head [0..idx-1] and tail [idx+1..end], installs new argv. */
int	replace_argv_segment(t_cmd *cmd, int idx, char **ms, size_t n)
{
	size_t	head;
	size_t	tail;
	size_t	oc;
	size_t	o;
	char	**nv;

	oc = argv_count(cmd->argv);
	head = (size_t)idx;
	tail = oc - head - 1;
	nv = (char **)pool_malloc(cmd->pool, (oc - 1 + n + 1) * sizeof(char *));
	if (!nv)
		return (-1);
	o = copy_ptr_block(nv, 0, cmd->argv, head);
	if (n == 0)
		nv[o++] = cmd->argv[idx];
	else
		o = copy_ptr_block(nv, o, ms, n);
	o = copy_ptr_block(nv, o, cmd->argv + idx + 1, tail);
	nv[o] = NULL;
	cmd->argv = nv;
	return (0);
}
