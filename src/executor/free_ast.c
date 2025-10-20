/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:21:56 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/10 11:21:57 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "proof.h"

/*
** executor_free_redirs: release a linked list of redirection nodes.
** Inputs: head pointer to the redirection list.
** Outputs: none.
** Side effects: closes heredoc fds and frees target strings.
*/
void	executor_free_redirs(t_redir *list)
{
	while (list)
	{
		if (list->kind == R_HDOC && list->fd >= 0)
			close(list->fd);
		list = list->next;
	}
}

/*
** executor_free_cmd: dispose of a command node including argv and redirs.
** Inputs: pointer to the command structure.
** Outputs: none.
** Side effects: frees argv entries and associated redirection list.
*/
void	executor_free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	executor_free_redirs(cmd->redirs);
}

/*
** executor_free_ast: recursively release an AST and its owned resources.
** Inputs: root pointer of the AST to destroy.
** Outputs: none.
** Side effects: frees nested command nodes and redirection structures.
*/
void	executor_free_ast(t_ast *node)
{
	if (!node)
		return ;
	executor_free_redirs(node->redirs);
	if (node->kind == NK_CMD)
		executor_free_cmd(node->cmd);
	else if (node->kind == NK_GROUP)
		executor_free_ast(node->child);
	else
	{
		executor_free_ast(node->left);
		executor_free_ast(node->right);
	}
}
