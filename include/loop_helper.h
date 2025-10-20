/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_helper.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:34:18 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 12:55:10 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "minishell.h"

int		is_interactive(void);
char	*read_one_line(t_ctx *ctx, int interactive);
void	cleanup_prompt(t_ctx *ctx, int interactive);
int		validate_quotes(t_ctx *ctx, char **line, int interactive);
