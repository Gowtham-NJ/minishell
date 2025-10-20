/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_apply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:13:40 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:22:32 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pool.h"
#include "proof.h"
#include "status.h"

char		*status_replace_word_pool(t_pool *pool, const char *word,
				const char *status, size_t status_len);

static int	update_redir_list(t_pool *pool, t_redir *list, const char *status,
		size_t status_len)
{
	char	*updated;

	while (list)
	{
		updated = status_replace_word_pool(pool, list->target, status,
				status_len);
		if (!updated)
			return (-1);
		list->target = updated;
		list = list->next;
	}
	return (0);
}

static int	status_apply_cmd(t_pool *pool, t_cmd *cmd, const char *status,
		size_t status_len)
{
	int		index;
	char	*updated;
	t_pool	*use;

	if (!cmd)
		return (0);
	index = 0;
	while (cmd->argv && cmd->argv[index])
	{
		if (cmd->pool)
			use = cmd->pool;
		else
			use = pool;
		updated = status_replace_word_pool(use, cmd->argv[index], status,
				status_len);
		if (!updated)
			return (-1);
		cmd->argv[index] = updated;
		index += 1;
	}
	return (update_redir_list(pool, cmd->redirs, status, status_len));
}

int	status_apply_exit(t_pool *pool, t_ast *node, const char *status,
		size_t status_len)
{
	if (!node)
		return (0);
	if (update_redir_list(pool, node->redirs, status, status_len) != 0)
		return (-1);
	if (node->kind == NK_CMD)
		return (status_apply_cmd(pool, node->cmd, status, status_len));
	if (node->kind == NK_GROUP)
		return (status_apply_exit(pool, node->child, status, status_len));
	if (status_apply_exit(pool, node->left, status, status_len) != 0)
		return (-1);
	return (status_apply_exit(pool, node->right, status, status_len));
}
