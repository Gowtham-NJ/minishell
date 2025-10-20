/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:41 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/10 18:02:42 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "libft/libft.h"

static int	delimiter_length(const char *cursor)
{
	int	i;
	int	len;

	i = 0;
	while (cursor[i] != '\0' && ft_isspace((int)cursor[i]) != 0)
		i += 1;
	len = 0;
	while (cursor[i + len] != '\0' && ft_isspace((int)cursor[i + len]) == 0
		&& cursor[i + len] != '|' && cursor[i + len] != '<' && cursor[i
			+ len] != '>')
		len += 1;
	return (len);
}

static void	update_quote_state(char c, int *quote)
{
	if (*quote == 0 && (c == '\'' || c == '"'))
		*quote = (int)c;
	else if (*quote == (int)c)
		*quote = 0;
}

/*
** heredoc_count: count the number of heredoc redirections in a line.
** Inputs: command line string to inspect for heredoc tokens.
** Outputs: total heredoc occurrences found in the input string.
** Side effects: none; the original string remains untouched.
*/
int	heredoc_count(const char *line)
{
	int	count;
	int	i;
	int	quote;

	count = 0;
	i = 0;
	quote = 0;
	while (line[i] != '\0' && line[i + 1] != '\0')
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			update_quote_state(line[i], &quote);
			i += 1;
			continue ;
		}
		if (quote == 0 && line[i] == '<' && line[i + 1] == '<')
		{
			count += 1;
			i += 2;
		}
		else
			i += 1;
	}
	return (count);
}

/*
** heredoc_validate: ensure each heredoc token is followed by a delimiter.
** Inputs: command line string to inspect for heredoc structure.
** Outputs: returns 0 when all delimiters are valid, -1 otherwise.
** Side effects: none; purely reads from the provided buffer.
*/
int	heredoc_validate(const char *line)
{
	int	i;
	int	len;
	int	quote;

	i = 0;
	quote = 0;
	while (line[i] != '\0' && line[i + 1] != '\0')
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			update_quote_state(line[i], &quote);
			i += 1;
			continue ;
		}
		if (quote == 0 && line[i] == '<' && line[i + 1] == '<')
		{
			len = delimiter_length(line + i + 2);
			if (len <= 0)
				return (-1);
			i += 2;
			continue ;
		}
		i += 1;
	}
	return (0);
}
