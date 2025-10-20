/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:38:18 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/12 20:14:54 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "heredoc.h"

typedef struct s_collect
{
	t_heredoc	*docs;
	int			total;
	int			i;
	int			index;
	int			res;
	int			quote;
	char		**envp;
}				t_collect;

int				heredoc_child_collect(t_heredoc *doc, int pipe_fd[2],
					int lines);
int				heredoc_parent_receive(int fd, char **body);
int				heredoc_send_buffer(int fd, char *body);
int				heredoc_write_chunk(int fd, const void *buf, size_t len);
void			heredoc_print_eof(const char *delimiter, int lines);
char			*heredoc_read_body(t_heredoc *doc, char **envp, int lines);
