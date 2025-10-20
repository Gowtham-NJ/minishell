/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:01:53 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 19:32:32 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft/libft.h"
#include "status.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LLMAX 9223372036854775807
#define LLMIN -9223372036854775808

#define YES_OK 1
#define NOT_OK 0

typedef struct s_exec	t_exec;

typedef struct s_long_long
{
	int					sign;
	unsigned long long	temp;
	long long			num;
	int					flag;
}						t_long;

int						ft_exit_builtin(t_exec *exec_ctx, char **argv,
							int in_child);
t_long					ft_atoi_long_long_check(const char *s);
long long				ft_atoi_long_long(const char *s);
