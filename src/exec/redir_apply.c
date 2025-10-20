/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_apply.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:23:15 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/04 10:49:27 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "proof.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int	dup_into(int fd, int dest, int trace)
{
	int	rc;

	if (trace)
		ft_dprintf(2, "[dup2] fd%d -> fd%d\n", fd, dest);
	rc = dup2(fd, dest);
	if (rc < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_file_redir(t_exec *E, t_redir *r)
{
	int	fd;
	int	flags;

	if (r->kind == R_IN)
		fd = open(r->target, O_RDONLY);
	else
	{
		flags = O_WRONLY | O_CREAT;
		if (r->kind == R_APP)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(r->target, flags, 0644);
	}
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", r->target, strerror(errno));
		return (-1);
	}
	if (dup_into(fd, r->dest_fd, E->trace) < 0)
		return (-1);
	return (0);
}

int	apply_redirs(t_exec *E, t_redir *r)
{
	int	rc;

	while (r)
	{
		if (r->kind == R_IN || r->kind == R_OUT || r->kind == R_APP)
		{
			rc = handle_file_redir(E, r);
			if (rc < 0)
				return (-1);
		}
		else if (r->kind == R_HDOC)
		{
			if (E->trace)
				ft_dprintf(2, "[dup2] heredoc-fd -> fd%d\n", r->dest_fd);
			if (dup2(r->fd, r->dest_fd) < 0)
			{
				perror("dup2");
				return (-1);
			}
			close(r->fd);
		}
		r = r->next;
	}
	return (0);
}
