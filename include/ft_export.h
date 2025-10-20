/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:47:19 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/14 17:04:08 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**
 * This is here only because of the t_cmd
 */
#include "exec.h"

// ft_export.c ↓

int		ft_export(char ***env, char **cmd);

// ft_export_check.c ↓

int		ft_export_input_check(char *arg);

// ft_export_utils.c ↓
int		is_valid_ident(const char *s);
void	ft_export_swap_pointers(char **a, char **b);
void	ft_export_sort_copy(char ***copy, int len);
void	ft_export_free_copy(char **copy);
void	ft_export_print_loop(char *s);
char	*ft_export_get_name(t_pool *pool, const char *arg);
void	export_err(const char *arg);
int		env_set(char ***envp, const char *entry);
int		env_index_of(char **env, const char *key, int klen);
int		keylen(const char *s);
// ft_export_utils_2.c ↓
int		is_domain_entry(const char *s);
void	ft_export_sort_and_print(char **env);
void	ft_export_print_error(t_pool *pool, const char *arg);
int		ft_export_is_there(t_pool *pool, const char *arg, char ***env);
int		env_add(char *entry, char ***envp);
char	**env_copy(char **envp);
void	env_print_sorted_skip_domain(char **env);
void	env_print_skip_domain(char **env);
