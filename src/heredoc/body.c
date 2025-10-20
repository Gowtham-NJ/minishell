/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   body.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:20 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:27:50 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include "minishell.h"
#include "signal_setup.h"

/* child worker: never returns */
static void	run_heredoc_child(t_heredoc *doc, int pipe_fd[2], char **envp,
		int lines)
{
	t_pool	__pool;
	char	*argv_true[2];

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	pool_init(&__pool);
	close(pipe_fd[0]);
	if (heredoc_child_collect(doc, pipe_fd, lines) != 0)
	{
		close(pipe_fd[1]);
		pool_dispose(&__pool);
		exit(1);
	}
	close(pipe_fd[1]);
	argv_true[0] = (char *)"true";
	argv_true[1] = NULL;
	execve("/bin/true", argv_true, envp);
	pool_dispose(&__pool);
	exit(1);
}

pid_t	spawn_heredoc_child(t_heredoc *doc, int pipe_fd[2], char **envp,
		int lines)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		run_heredoc_child(doc, pipe_fd, envp, lines);
	return (pid);
}

static int	finish_parent_stage(int read_fd, pid_t pid, char **body,
		struct termios *snap)
{
	int	status;
	int	res;

	res = heredoc_parent_receive(read_fd, body);
	close(read_fd);
	waitpid(pid, &status, 0);
	signal_prepare_prompt();
	if (res != 0 || WIFSIGNALED(status))
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, snap);
		if (*body)
		{
			free(*body);
			*body = NULL;
		}
		if (WIFSIGNALED(status))
		{
			ft_putstr_fd("^C\n", STDERR_FILENO);
			signal_flag_set(SIGINT_HEREDOC);
		}
		return (-1);
	}
	return (0);
}

/*
** heredoc_read_body: orchestrate child/parent cooperation for heredoc capture.
** Inputs: heredoc descriptor and environment pointer forwarded to execve.
** Outputs: returns the allocated heredoc body, or NULL on failure/signal.
** Side effects: spawns a child process and updates the global signal flag.
*/
char	*heredoc_read_body(t_heredoc *doc, char **envp, int lines)
{
	int				pipe_fd[2];
	pid_t			pid;
	char			*body;
	struct termios	snap;

	tcgetattr(STDIN_FILENO, &snap);
	body = NULL;
	if (pipe(pipe_fd) < 0)
		return (NULL);
	pid = spawn_heredoc_child(doc, pipe_fd, envp, lines);
	if (pid < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (NULL);
	}
	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	if (finish_parent_stage(pipe_fd[0], pid, &body, &snap) != 0)
		return (NULL);
	return (body);
}
