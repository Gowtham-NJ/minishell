/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:10:51 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:00:53 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	update_quote(int ch, int *q)
{
	if (ch == '\'' && *q != 2)
	{
		if (*q == 1)
			*q = 0;
		else
			*q = 1;
		return (1);
	}
	if (ch == '"' && *q != 1)
	{
		if (*q == 2)
			*q = 0;
		else
			*q = 2;
		return (1);
	}
	return (0);
}

int	env_write_take(char *name, char **envp, t_buf *out)
{
	size_t	len;
	int		i;
	int		rc;

	if (!name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
		{
			rc = buf_putn(out, envp[i] + len + 1, ft_strlen(envp[i] + len + 1));
			return (rc);
		}
		i++;
	}
	return (0);
}

int	cs_span_step(const char *s, size_t *j, int *depth, int quote)
{
	if (s[*j] == '\\' && quote != 1 && s[*j + 1])
	{
		*j += 2;
		return (1);
	}
	if (quote == 0 && s[*j] == '(')
	{
		*j += 1;
		*depth += 1;
		return (1);
	}
	if (quote == 0 && s[*j] == ')')
	{
		*j += 1;
		*depth -= 1;
		return (2);
	}
	return (0);
}
