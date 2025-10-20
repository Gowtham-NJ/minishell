/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:28 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 17:47:28 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "heredoc_internal.h"
#include "minishell.h"

static void	release_partial(t_heredoc *docs, int used)
{
	int	index;

	index = 0;
	while (index < used)
	{
		free(docs[index].body);
		free(docs[index].delimiter);
		free(docs[index].delimiter_raw);
		index += 1;
	}
	free(docs);
}

static int	set_delimiter(t_heredoc *doc, const char *cursor, int index)
{
	int	start;
	int	len;

	start = 0;
	while (cursor[start] != '\0' && ft_isspace((int)cursor[start]) != 0)
		start += 1;
	len = 0;
	while (cursor[start + len] != '\0' && ft_isspace((int)cursor[start
				+ len]) == 0 && cursor[start + len] != '|' && cursor[start
			+ len] != '<' && cursor[start + len] != '>')
		len += 1;
	doc->index = index;
	doc->delimiter_raw = ft_substr(cursor, start, len);
	if (!doc->delimiter_raw)
		return (-1);
	doc->delimiter = ft_strtrim(doc->delimiter_raw, "\"'");
	if (!doc->delimiter)
		return ((free(doc->delimiter_raw)), -1);
	if (doc->delimiter_raw[0] == '\'' || doc->delimiter_raw[0] == '"')
		doc->expand = HEREDOC_NO_EXPAND;
	else
		doc->expand = HEREDOC_DO_EXPAND;
	return (0);
}

static int	fill_doc_entry_c(t_collect *c, const char *cursor, int lines)
{
	t_heredoc	*doc;
	int			status;

	doc = &c->docs[c->index];
	status = set_delimiter(doc, cursor, c->index);
	if (status != 0)
		return (-1);
	doc->body = heredoc_read_body(doc, c->envp, lines);
	if (!doc->body)
	{
		free(doc->delimiter);
		free(doc->delimiter_raw);
		doc->delimiter = NULL;
		doc->delimiter_raw = NULL;
		return (-1);
	}
	return (0);
}

// scanner: walks the line, updates collector, fills docs (0 ok, -1 err)
static int	collect_scan(t_collect *c, const char *s, int lines)
{
	while (s[c->i] && s[c->i + 1] && c->index < c->total)
	{
		if (s[c->i] == '\'' || s[c->i] == '"')
		{
			if (c->quote == 0)
				c->quote = (int)s[c->i];
			else if (c->quote == (int)s[c->i])
				c->quote = 0;
			c->i++;
			continue ;
		}
		if (c->quote == 0 && s[c->i] == '<' && s[c->i + 1] == '<')
		{
			c->res = fill_doc_entry_c(c, s + c->i + 2, lines);
			if (c->res != 0)
				return (-1);
			c->index++;
			c->i += 2;
			continue ;
		}
		c->i++;
	}
	return (0);
}

/*
** heredoc_collect: build the heredoc descriptor array by scanning the line.
** Inputs: command line and inherited environment pointer for child execve.
** Outputs: returns allocated array on success, NULL if interrupted or failed.
** Side effects: spawns heredoc readers and allocates delimiter/body storage.
*/
t_heredoc	*heredoc_collect(const char *line, char **envp, int lines)
{
	t_collect	c;

	c.total = heredoc_count(line);
	if (c.total <= 0)
		return (NULL);
	c.docs = (t_heredoc *)ft_calloc((size_t)c.total, sizeof(t_heredoc));
	if (!c.docs)
		return (NULL);
	c.i = 0;
	c.index = 0;
	c.quote = 0;
	c.envp = envp;
	if (collect_scan(&c, line, lines) != 0)
	{
		release_partial(c.docs, c.index);
		return (NULL);
	}
	return (c.docs);
}
