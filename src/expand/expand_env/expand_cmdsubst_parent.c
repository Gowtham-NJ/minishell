/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmdsubst_parent.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:13:14 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/14 23:15:26 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

static int	cs_pipe_fork(int pipe_fd[2], pid_t *pid)
{
	if (pipe(pipe_fd) < 0)
		return (-1);
	*pid = fork();
	if (*pid < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	return (0);
}

static int	cs_read_into_buf(int fd, t_buf *buf)
{
	char	tmp[4096];
	ssize_t	rd;

	while (1)
	{
		rd = read(fd, tmp, sizeof(tmp));
		if (rd > 0)
		{
			if (buf_putn(buf, tmp, (size_t)rd) != 0)
				return (-1);
			continue ;
		}
		if (rd < 0 && errno == EINTR)
			continue ;
		if (rd < 0)
			return (-1);
		break ;
	}
	return (0);
}

static void	cs_wait(pid_t pid, int *st)
{
	int	status;

	while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
		;
	if (st)
		*st = status;
}

static char	*cs_parent_collect_and_wait(t_pool *pool, int rfd, pid_t pid)
{
	t_buf	buf;
	int		status;

	if (buf_init(&buf, pool, 128) != 0)
	{
		close(rfd);
		cs_wait(pid, NULL);
		return (NULL);
	}
	if (cs_read_into_buf(rfd, &buf) != 0)
	{
		close(rfd);
		cs_wait(pid, NULL);
		return (NULL);
	}
	close(rfd);
	cs_wait(pid, &status);
	while (buf.len > 0 && buf.data[buf.len - 1] == '\n')
		buf.len--;
	return (buf_take(&buf));
}

char	*command_subst_eval(t_exp_ctx *c, const char *body)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!c->exec_ctx)
		return (NULL);
	if (cs_pipe_fork(pipe_fd, &pid) < 0)
		return (NULL);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		command_subst_exec_child(c->exec_ctx, body, pipe_fd[1]);
	}
	close(pipe_fd[1]);
	return (cs_parent_collect_and_wait(c->exec_ctx->pool, pipe_fd[0], pid));
}
