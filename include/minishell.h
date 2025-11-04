/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:39:40 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/18 10:58:32 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include "ctx.h"
# include "libft/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef enum e_sigflag
{
	NO_SIGINT = 0,
	SIGINT_MAIN = 1,
	SIGINT_HEREDOC = 2,
	SIGINT_QUOTES = 3
}		t_sigflag;

int		signal_flag_get(void);
void	signal_flag_set(t_sigflag flag);
void	signal_flag_clear(void);

int		shell_loop(t_ctx *ctx);
void	ft_copy_argv(char **argv, char ***tmp, size_t argc);
// loop_utils.c helpers

void	ft_add_history_count_lines(t_ctx *ctx, char *line);
int		ft_count_lines(const char *line);
int		ft_handle_minishell_exit(t_ctx *ctx);

#endif
