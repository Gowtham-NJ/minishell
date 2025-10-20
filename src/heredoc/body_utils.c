/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   body_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:46 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/12 20:05:49 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include "minishell.h"

/*
** heredoc_print_eof: mimic bash warning when EOF arrives before delimiter.
** Inputs: heredoc index for numbering and the expected delimiter string.
** Outputs: none.
** Side effects: writes a diagnostic message to standard error.
*/
void	heredoc_print_eof(const char *delimiter, int lines)
{
	size_t	len;

	len = ft_strlen(delimiter);
	ft_dprintf(STDERR_FILENO, "minishell: warning: here-document at line ");
	ft_dprintf(STDERR_FILENO, "%i delimited by end-of-file", lines);
	ft_dprintf(STDERR_FILENO, " (wanted `%s')\n", delimiter);
}

/*
** heredoc_write_chunk: send an exact byte count over a pipe with EINTR guard.
** Inputs: destination file descriptor, source buffer pointer, and length.
** Outputs: returns 0 on success, -1 when the write cannot complete.
** Side effects: writes to the provided descriptor; no buffer mutations occur.
*/
int	heredoc_write_chunk(int fd, const void *buf, size_t len)
{
	size_t		progress;
	ssize_t		written;
	const char	*bytes;

	progress = 0;
	bytes = (const char *)buf;
	while (progress < len)
	{
		written = write(fd, bytes + progress, len - progress);
		if (written > 0)
			progress += (size_t)written;
		else if (written < 0 && errno == EINTR)
			continue ;
		else
			return (-1);
	}
	return (0);
}

/*
** heredoc_send_buffer: stream heredoc length header and body to the parent.
** Inputs: destination descriptor and the prepared heredoc body string.
** Outputs: returns 0 on success, -1 if any write fails.
** Side effects: writes length header and payload; body remains owned by caller.
*/
int	heredoc_send_buffer(int fd, char *body)
{
	unsigned int	len;
	int				res;

	len = 0;
	if (body)
		len = (unsigned int)ft_strlen(body);
	res = heredoc_write_chunk(fd, &len, sizeof(unsigned int));
	if (res != 0)
		return (-1);
	if (len == 0)
		return (0);
	res = heredoc_write_chunk(fd, body, len);
	if (res != 0)
		return (-1);
	return (0);
}
