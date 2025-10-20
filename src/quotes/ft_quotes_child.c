/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:02:33 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/15 12:55:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_quotes.h"

static int	append_with_nl(char **dst, const char *src)
{
	char	*old;
	char	*tmp;
	char	*joined;

	if (!dst || !src)
		return (-1);
	old = *dst;
	tmp = ft_strjoin(old, "\n");
	if (!tmp)
		return (-1);
	joined = ft_strjoin(tmp, src);
	free(tmp);
	if (!joined)
		return (-1);
	*dst = joined;
	free(old);
	return (0);
}

static int	ft_quotes_write_buffer(int fd, const void *buff, size_t len)
{
	size_t		progress;
	ssize_t		written;
	const char	*bytes;

	progress = 0;
	bytes = (const char *)buff;
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

static int	ft_quotes_write_to_pipe(int fd_write, char *new_line)
{
	unsigned int	len;
	int				res;

	if (new_line)
		len = ft_strlen(new_line);
	res = ft_quotes_write_buffer(fd_write, &len, sizeof(unsigned int));
	if (res != 0)
	{
		free(new_line);
		return (-1);
	}
	if (len == 0)
	{
		free(new_line);
		return (-1);
	}
	res = ft_quotes_write_buffer(fd_write, new_line, len);
	if (res != 0)
	{
		free(new_line);
		return (-1);
	}
	free(new_line);
	return (0);
}

/**
 * actual reading and checking here
 */
int	ft_quote_read_lines(char *line, int fd_write)
{
	char	*assembled;
	char	*next;

	assembled = ft_strdup(line);
	if (!assembled)
		return (-1);
	free(line);
	while (1)
	{
		next = readline("> ");
		if (!next)
			return ((free(assembled)), -1);
		if (append_with_nl(&assembled, next) != 0)
		{
			free(next);
			free(assembled);
			return (-1);
		}
		free(next);
		if (input_check_quotes(assembled) == 0)
			break ;
	}
	if (ft_quotes_write_to_pipe(fd_write, assembled) != 0)
		return (-1);
	return (0);
}
