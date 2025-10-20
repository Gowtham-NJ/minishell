/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   body_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:50 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/10 22:47:11 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include "minishell.h"

static int	read_exact(int fd, void *buf, size_t len)
{
	size_t	filled;
	ssize_t	count;
	char	*bytes;

	filled = 0;
	bytes = (char *)buf;
	while (filled < len)
	{
		count = read(fd, bytes + filled, len - filled);
		if (count > 0)
			filled += (size_t)count;
		else if (count == 0)
			return (-1);
		else if (errno != EINTR)
			return (-1);
	}
	return (0);
}

static int	set_empty_body(char **body)
{
	*body = ft_strdup("");
	if (!*body)
		return (-1);
	return (0);
}

/*
** heredoc_parent_receive: read heredoc data from the child pipe endpoint.
** Inputs: read descriptor and output pointer for the allocated body string.
** Outputs: returns 0 on success, -1 if the stream ends unexpectedly.
** Side effects: allocates the body buffer; caller must free the result.
*/
int	heredoc_parent_receive(int fd, char **body)
{
	unsigned int	len;
	int				res;

	len = 0;
	res = read_exact(fd, &len, sizeof(unsigned int));
	if (res != 0)
		return (-1);
	if (len == 0)
		return (set_empty_body(body));
	*body = (char *)malloc((size_t)len + 1);
	if (!*body)
		return (-1);
	res = read_exact(fd, *body, len);
	if (res != 0)
	{
		free(*body);
		*body = NULL;
		return (-1);
	}
	(*body)[len] = '\0';
	return (0);
}
