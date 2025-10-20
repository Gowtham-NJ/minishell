/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 22:09:34 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/12 20:14:43 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_QUOTES_H
# define FT_QUOTES_H

# include "exec.h"
# include "executor.h"
# include "heredoc.h"
# include "input.h"
# include "minishell.h"
# include "signal_setup.h"
# include <signal.h>
# include <sys/wait.h>
# include <termios.h>

int		ft_quotes_allow_more(char **line, char *prompt, char **env);

void	ft_quotes_write_error(void);
int		ft_quotes_read_len(int read_fd, int *len);
int		ft_quotes_read_str(int read_fd, int len, char **str);

int		ft_quote_read_lines(char *line, int fd_write);

#endif
