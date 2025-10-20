/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:18:12 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 16:28:37 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	append_targets_from_list(t_redir *list, t_redir **targets, int total,
		int *index)
{
	while (list)
	{
		if (list->kind == R_HDOC)
		{
			if (*index >= total)
				return (-1);
			targets[*index] = list;
			*index += 1;
		}
		list = list->next;
	}
	return (0);
}

int	collect_targets(t_ast *node, t_redir **targets, int total, int *index)
{
	if (!node)
		return (0);
	if (append_targets_from_list(node->redirs, targets, total, index) != 0)
		return (-1);
	if (node->kind == NK_CMD)
	{
		if (node->cmd && append_targets_from_list(node->cmd->redirs, targets,
				total, index) != 0)
			return (-1);
		return (0);
	}
	if (node->kind == NK_GROUP)
		return (collect_targets(node->child, targets, total, index));
	if (collect_targets(node->left, targets, total, index) != 0)
		return (-1);
	return (collect_targets(node->right, targets, total, index));
}

int	attach_single_doc(t_redir *target, t_heredoc *doc)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		return (-1);
	if (doc->body && pipe_write_all(pipe_fd[1], doc->body) != 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	target->fd = pipe_fd[0];
	return (0);
}
