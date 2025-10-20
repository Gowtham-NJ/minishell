/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:17:26 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/10 19:48:13 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static const char	*kind(t_node_kind newk)
{
	if (newk == NK_CMD)
		return ("CMD");
	if (newk == NK_PIPE)
		return ("PIPE");
	if (newk == NK_AND)
		return ("AND");
	if (newk == NK_OR)
		return ("OR");
	if (newk == NK_LIST)
		return ("LIST");
	if (newk == NK_GROUP)
		return ("GROUP");
	return ("?");
}

void	print_ast(const t_ast *n, int d)
{
	int	i;

	if (!n)
		return ;
	ft_dprintf(2, "%*s%s", d, "", kind(n->kind));
	if (n->kind == NK_CMD)
	{
		ft_dprintf(2, " :");
		i = 0;
		if (n->cmd && n->cmd->argv)
		{
			while (n->cmd->argv[i])
				ft_dprintf(2, " %s", n->cmd->argv[i++]);
		}
		ft_dprintf(2, "\n");
		return ;
	}
	write(2, "\n", 1);
	if (n->kind == NK_GROUP)
	{
		print_ast(n->child, d + 2);
		return ;
	}
	print_ast(n->left, d + 2);
	print_ast(n->right, d + 2);
}
