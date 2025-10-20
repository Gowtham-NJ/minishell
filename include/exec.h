/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:45 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:58:48 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "executor.h"
#include "ft_exit.h"
#include "libft/libft.h"
#include "proof.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pool.h"

typedef struct s_fd_save
{
	int	in;
	int	out;
	int	err;
}		t_fd_save;

char	**dup_argv(t_pool *pool, char **v);
int		bi_pwd(char **argv);
int		eval_group_no_subshell(t_exec *E, t_ast *g);
int		eval_node(t_exec *E, t_ast *n);
int		execute_pipeline_plan(t_exec *E, t_pipeline_plan *pp);
void	fd_save_end(t_fd_save *s);
int		eval_group_subshell(t_exec *E, t_ast *group);
char	**ft_make_env_copy(char **envp, size_t wanted_len);
size_t	ft_get_env_len(char **envp);
int		ft_cd(char **cmd, char ***env, t_pool *pool);
int		ft_env(char **cmd, char **env);
int		ft_export(char ***env, char **cmd);
int		ft_echo(char **argv);
int		ft_unset(char **argv, char ***env, t_pool *pool);
char	*ft_make_prompt(char ***env);
char	*ft_get_env_val(char **env, char *name);
void	ft_set_domain(char *hostname, char ***env);
int		append(char **dst, const char *s);
/* assignment words */
int		is_assignment_word(const char *s);
int		assign_prefix_len(char **argv);
int		eval_cmd_node(t_exec *e, t_ast *n);
int		run_builtin_with_redirs(t_exec *E, t_cmd *c, char **argv);
int		spawn_simple_child(t_exec *E, t_cmd *c, char **argv);
