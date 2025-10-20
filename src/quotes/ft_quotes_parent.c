/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_parent.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 20:54:35 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 13:31:08 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_quotes.h"

static pid_t	ft_spawn_quote_child(char *line, int fd_pipe[2], char *prompt,
		char **env)
{
	pid_t	pid;
	t_pool	pool;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		ft_free_env_copy(env);
		free(prompt);
		signal_prepare_child();
		pool_init(&pool);
		close(fd_pipe[0]);
		ft_quote_read_lines(line, fd_pipe[1]);
		close(fd_pipe[1]);
		pool_dispose(&pool);
		exit(0);
	}
	return (pid);
}

static int	ft_quotes_receive(int read_fd, char **line)
{
	unsigned int	len;
	int				res;
	char			*new_str;
	char			*temp;

	len = 0;
	res = (unsigned int)ft_quotes_read_len(read_fd, (int *)&len);
	if (res != 0)
		return (-1);
	if (len == 0)
		return (-1);
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (-1);
	res = ft_quotes_read_str(read_fd, len, &new_str);
	if (res != 0)
	{
		free(new_str);
		return (-1);
	}
	new_str[len] = '\0';
	temp = *line;
	*line = new_str;
	free(temp);
	return (0);
}

static int	ft_quotes_allow_more_2(int read_fd, pid_t pid, char **line,
		struct termios *snap)
{
	int	status;
	int	res;

	res = ft_quotes_receive(read_fd, line);
	close(read_fd);
	waitpid(pid, &status, 0);
	signal_prepare_prompt();
	if (res != 0 || WIFSIGNALED(status))
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, snap);
		if (WIFSIGNALED(status))
		{
			signal_flag_set(SIGINT_QUOTES);
			ft_putstr_fd("^C\n", STDERR_FILENO);
		}
		else
			ft_quotes_write_error();
		return (-1);
	}
	return (0);
}

/**
 * New signal handler
 *
 * parent and child behaviour -> copy from heredoc
 *
 * parent forks, creates a pipe and reads from it as in heredoc
 *
 * child figures out the current state and reprompts as long
 * as all quotes are not closed. Then it writes len and the string
 * into the pipe. Frees all and exits
 *
 * The parent reads the int, then allocates space, reads from the pipe.
 *
 * Error behaviour: child has sig handler, on eof (ctrl d),it updates
 * the status. If that happens, the parent detects it, prints error
 * msgs and returns (-1) or something.
 *
 * Then the line proceeds to be freed by the normal pathway. Need to also flush
 * stdin as in heredoc
 */
int	ft_quotes_allow_more(char **line, char *prompt, char **env)
{
	struct termios	snap;
	int				fd_pipe[2];
	pid_t			pid;

	tcgetattr(STDIN_FILENO, &snap);
	if (pipe(fd_pipe) < 0)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = ft_spawn_quote_child(*line, fd_pipe, prompt, env);
	if (pid < 0)
	{
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		return (-1);
	}
	close(fd_pipe[1]);
	return (ft_quotes_allow_more_2(fd_pipe[0], pid, line, &snap));
}
