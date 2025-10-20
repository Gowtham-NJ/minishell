/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:23:48 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/12 10:51:53 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"

static int	expand_ast_node(t_ast *node)
{
	if (!node)
		return (0);
	if (node->kind == NK_CMD)
	{
		if (expand_globs_in_cmd(node->cmd) < 0)
			return (-1);
		return (0);
	}
	if (node->kind == NK_GROUP)
	{
		if (expand_ast_node(node->child) < 0)
			return (-1);
		return (0);
	}
	if (expand_ast_node(node->left) < 0)
		return (-1);
	if (expand_ast_node(node->right) < 0)
		return (-1);
	return (0);
}

int	expand_ast(t_ast *root)
{
	return (expand_ast_node(root));
}
