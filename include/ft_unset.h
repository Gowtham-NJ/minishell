/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:26:07 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 19:32:44 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "exec.h"

// ft_unset_check.c ↓

int		ft_unset_input_check(char *arg);

// ft_unset_utils.c ↓

char	*ft_unset_get_name(t_pool *pool, const char *arg);
int		ft_unset_is_there(t_pool *pool, const char *arg, char ***env);
void	ft_unset_free_copy(char **copy);

// ft_unset.c ↓

int		ft_unset(char **argv, char ***env, t_pool *pool);
