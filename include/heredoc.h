/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:38:00 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/12 19:48:17 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <sys/types.h>

# define HEREDOC_NO_EXPAND 0
# define HEREDOC_DO_EXPAND 1

typedef struct s_heredoc
{
	int		index;
	char	*delimiter_raw;
	char	*delimiter;
	int		expand;
	char	*body;
}			t_heredoc;

int			heredoc_count(const char *line);
int			heredoc_validate(const char *line);
t_heredoc	*heredoc_collect(const char *line, char **envp, int lines);
void		heredoc_release(t_heredoc *docs, int count);

#endif
