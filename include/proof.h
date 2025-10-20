/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proof.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:21 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/18 13:13:43 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "pool.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* ====================== AST ====================== */
typedef enum e_node_kind
{
	NK_CMD,
	NK_PIPE,
	NK_AND,
	NK_OR,
	NK_LIST,
	NK_GROUP
}						t_node_kind;

typedef enum e_redir_kind
{
	R_IN,
	R_OUT,
	R_APP,
	R_HDOC
}						t_redir_kind;

typedef struct s_redir
{
	t_redir_kind		kind;
	char				*target;
	int					fd;
	int					dest_fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_cmd
{
	char				**argv;
	unsigned char		*glob_ok;
	t_redir				*redirs;
	int					is_builtin;
	t_pool				*pool;
}						t_cmd;

typedef struct s_ast
{
	t_node_kind			kind;
	struct s_ast		*right;
	struct s_ast		*left;
	struct s_ast		*child;
	t_cmd				*cmd;
	t_redir				*redirs;
}						t_ast;

/* =================== Execution model =================== */
/* execution allocation pool (points to ctx->pool in parent;
   children set this to a local pool before _exit) */
typedef struct s_exec
{
	char				***envp;
	int					last_status;
	int					trace;
	struct s_fd_save	*sv;
	struct s_ctx		*ctx;
	char				*line;
	t_ast				*ast;
	char				*prompt;
	t_pool				*pool;
}						t_exec;

typedef enum e_stage_node_kind
{
	STAGE_NODE_CMD,
	STAGE_NODE_GROUP
}						t_stage_node_kind;

typedef struct s_child_plan
{
	t_stage_node_kind	k;
	t_ast				*node;
	t_cmd				*cmd;
	t_redir				*redirs;
	int					in_fd;
	int					out_fd;
	int					close_list[32];
	int					close_count;
	int					index;
	int					total;
}						t_child_plan;

typedef struct s_pipeline_plan
{
	t_child_plan		*stages;
	int					nstages;
	int					*parent_close;
	int					parent_close_n;
	int					(*pipes)[2];
}						t_pipeline_plan;

/* =================== Helpers / API =================== */
/* AST builders (allocate from provided pool) */
t_ast					*ast_cmd(t_pool *pool, char **argv);
t_ast					*ast_pipe(t_pool *pool, t_ast *a, t_ast *b);
t_ast					*ast_and(t_pool *pool, t_ast *a, t_ast *b);
t_ast					*ast_or(t_pool *pool, t_ast *a, t_ast *b);
t_ast					*ast_list(t_pool *pool, t_ast *a, t_ast *b);
t_ast					*ast_group(t_pool *pool, t_ast *inside);
t_redir					*redir_new(t_pool *pool, t_redir_kind k,
							const char *target, int dest_fd);
t_redir					*redir_push(t_redir *head, t_redir *r);
t_ast					*group_with_redir(t_ast *grp, t_redir *r);
t_ast					*node(t_pool *pool, t_node_kind k);
/* builtin helpers */
int						is_builtin(const char *name);
int						run_builtin_parent(t_exec *E, char **argv);
int						run_builtin_child(t_exec *E, char **argv);

/* planning & execution */
int						build_pipeline_plan(t_exec *E, t_ast *node,
							t_pipeline_plan *pp);
int						execute_pipeline_plan(t_exec *E, t_pipeline_plan *pp);
int						run_ast(t_exec *E, t_ast *n);

/* printing */
void					print_ast(const t_ast *n, int d);

/* small utils */
char					**mk_argv(t_pool *pool, const char *a0, ...);
int						apply_redirs(t_exec *E, t_redir *r);

int						is_path_with_slash(const char *s);
int						resolve_command(char *dst, size_t dstsz,
							const char *cmd, char *const envp[]);
int						print_exec_error_and_code(const char *path, int err);
int						exec_with_resolution(t_exec *E, char **argv);
int						syntax_error_unexpected(const char *tok);
int						syntax_error_eof(void);

void					ft_def_child_free(t_exec *E);
void					ft_child_on_err_free(t_exec *E);
