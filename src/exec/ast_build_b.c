/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:16:58 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 12:46:56 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_ast	*ast_list(t_pool *pool, t_ast *a, t_ast *b)
{
	t_ast	*n;

	n = (t_ast *)pool_calloc(pool, 1, sizeof(*n));
	if (!n)
		return (NULL);
	n->kind = NK_LIST;
	n->left = a;
	n->right = b;
	return (n);
}

t_ast	*ast_group(t_pool *pool, t_ast *inside)
{
	t_ast	*n;

	n = (t_ast *)pool_calloc(pool, 1, sizeof(*n));
	if (!n)
		return (NULL);
	n->kind = NK_GROUP;
	n->child = inside;
	return (n);
}

t_redir	*redir_new(t_pool *pool, t_redir_kind k, const char *target,
		int dest_fd)
{
	t_redir	*r;

	r = (t_redir *)pool_calloc(pool, 1, sizeof(*r));
	if (!r)
		return (NULL);
	r->kind = k;
	if (target)
		r->target = pool_strdup(pool, (char *)target);
	else
		r->target = NULL;
	r->fd = -1;
	r->dest_fd = dest_fd;
	r->next = NULL;
	return (r);
}

t_redir	*redir_push(t_redir *head, t_redir *r)
{
	if (!r)
		return (head);
	r->next = head;
	return (r);
}

t_ast	*group_with_redir(t_ast *grp, t_redir *r)
{
	grp->redirs = redir_push(grp->redirs, r);
	return (grp);
}
