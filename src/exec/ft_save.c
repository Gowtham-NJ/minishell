/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_save.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:24:41 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/09 22:57:13 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	fd_save_end(t_fd_save *s)
{
	if (s->in != -1)
	{
		dup2(s->in, 0);
		close(s->in);
	}
	if (s->out != -1)
	{
		dup2(s->out, 1);
		close(s->out);
	}
	if (s->err != -1)
	{
		dup2(s->err, 2);
		close(s->err);
	}
}

int	eval_group_no_subshell(t_exec *E, t_ast *g)
{
	t_fd_save	sv;
	int			st;

	sv.in = dup(0);
	sv.out = dup(1);
	sv.err = dup(2);
	if (apply_redirs(E, g->redirs) != 0)
	{
		fd_save_end(&sv);
		return (1);
	}
	st = eval_node(E, g->child);
	fd_save_end(&sv);
	return (st);
}
