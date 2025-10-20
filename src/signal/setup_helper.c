/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:49:15 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 14:13:58 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "signal_setup.h"
#include <string.h>

static volatile sig_atomic_t	*sig_slot(void)
{
	static volatile sig_atomic_t	s;

	return (&s);
}

int	signal_flag_get(void)
{
	return ((int)*sig_slot());
}

void	signal_flag_set(t_sigflag flag)
{
	*sig_slot() = flag;
}

void	signal_flag_clear(void)
{
	*sig_slot() = NO_SIGINT;
}
