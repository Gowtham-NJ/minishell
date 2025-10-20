/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:30:05 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/14 23:30:05 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

int	is_word_delim(int c);
int	skip_quoted(char **pp);
int	try_special_cmdsubst(char **p, int *depth, int q);
