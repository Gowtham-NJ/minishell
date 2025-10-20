/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:36:10 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 16:30:58 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "ctx.h"
# include "expand.h"
# include "heredoc.h"
# include "input.h"
# include "libft/libft.h"
# include "minishell.h"
# include "parse.h"
# include "proof.h"
# include "signal_setup.h"
# include "status.h"
# include <errno.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_ex_run
{
	t_parser	p;
	t_ast		*root;
	const char	*tok;
}				t_ex_run;

void			executor_run(t_exec *exec_ctx, t_ctx *ctx, const char *line);
// void	executor_report_parse_error(t_parser *parser);
int				executor_attach_heredocs(t_exec *E, t_ast *root,
					t_heredoc *docs, int doc_count);
void			executor_free_redirs(t_redir *list);
void			executor_free_cmd(t_cmd *cmd);
void			executor_free_ast(t_ast *node);
int				executor_eval_pipeline(t_exec *exec_ctx, t_ast *node);
int				collect_targets(t_ast *node, t_redir **targets, int total,
					int *index);
int				append_targets_from_list(t_redir *list, t_redir **targets,
					int total, int *index);
int				attach_single_doc(t_redir *target, t_heredoc *doc);
int				pipe_write_all(int fd, const char *body);
#endif
