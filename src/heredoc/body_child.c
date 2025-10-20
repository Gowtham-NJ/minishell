/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   body_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:53 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 19:30:38 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_internal.h"
#include "minishell.h"

static char	*append_body(char *body, char *temp)
{
	char	*joined;

	if (!temp)
		return (NULL);
	if (!body)
		return (temp);
	joined = ft_strjoin(body, temp);
	free(body);
	free(temp);
	if (!joined)
		return (NULL);
	return (joined);
}

static char	*handle_line_missing(char *body, t_heredoc *doc, int lines,
		int flag)
{
	if (!body)
	{
		body = ft_strdup("");
		if (!body)
			return (NULL);
	}
	if (flag == 1)
		heredoc_print_eof(doc->delimiter, lines);
	return (body);
}

static char	*collect_body_loop(t_heredoc *doc, int lines)
{
	char	*body;
	char	*line;
	char	*temp;
	int		line_count;

	body = NULL;
	line_count = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (handle_line_missing(body, doc, lines, 1));
		if (ft_strcmp(line, doc->delimiter) == 0 && line_count == 0)
			return (handle_line_missing(body, doc, lines, 2));
		if (ft_strcmp(line, doc->delimiter) == 0)
			return ((free(line)), body);
		temp = ft_strjoin(line, "\n");
		free(line);
		body = append_body(body, temp);
		if (!body)
			return (NULL);
		line_count++;
	}
}

/*
** heredoc_child_collect: capture heredoc input and stream it to the parent.
** Inputs: heredoc descriptor for metadata and the child-side pipe handles.
** Outputs: returns 0 on success, -1 when capturing or sending fails.
** Side effects: allocates the heredoc body and writes it through the pipe.
*/
int	heredoc_child_collect(t_heredoc *doc, int pipe_fd[2], int lines)
{
	char	*body;

	body = collect_body_loop(doc, lines);
	if (!body)
		return (-1);
	if (heredoc_send_buffer(pipe_fd[1], body) != 0)
	{
		free(body);
		return (-1);
	}
	free(body);
	return (0);
}
