/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:36:26 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 10:42:14 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include "exec.h"
#include "heredoc.h"
#include "libft/libft.h"

static void	ctx_zero(t_ctx *c)
{
	if (c)
		ft_bzero(c, sizeof(*c));
}

int	ctx_begin(t_ctx *c, char **envp)
{
	if (!c || !envp)
		return (-1);
	ctx_zero(c);
	pool_init(&c->pool);
	c->envp = env_copy(envp);
	c->lines_so_far = 1;
	c->lines_so_far = 1;
	if (!c->envp)
		return (-1);
	c->last_status = 0;
	return (0);
}

void	ctx_reset(t_ctx *c)
{
	if (!c)
		return ;
	c->ast = NULL;
	c->heredocs = NULL;
	c->heredoc_count = 0;
}

void	ctx_end(t_ctx *c)
{
	if (!c)
		return ;
	if (c->heredocs && c->heredoc_count > 0)
		heredoc_release(c->heredocs, c->heredoc_count);
	c->heredocs = NULL;
	c->heredoc_count = 0;
	ft_free_env_copy(c->envp);
	c->envp = NULL;
	c->last_status = 0;
	c->ast = NULL;
	free(c->prompt);
	ft_bzero(&c->snap, sizeof(c->snap));
	pool_dispose(&c->pool);
}
