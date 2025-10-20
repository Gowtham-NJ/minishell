/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:24:01 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 18:05:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "proof.h"
#include <stdio.h>

int	syntax_error_unexpected(const char *tok)
{
	const char	*s;

	s = tok;
	if (!s)
		s = "newline";
	ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n", s);
	return (2);
}

int	syntax_error_eof(void)
{
	ft_dprintf(2, "minishell: syntax error: unexpected end of file\n");
	return (2);
}
