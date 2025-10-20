/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:55:29 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 12:34:46 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_export.h"
#include <stdlib.h>

int	keylen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

int	env_index_of(char **env, const char *key, int klen)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], key, klen) == 0 && (env[i][klen] == '\0'
			|| env[i][klen] == '='))
			return (i);
		i++;
	}
	return (-1);
}

int	is_valid_ident(const char *s)
{
	int	i;

	if (!s || s[0] == '\0' || s[0] == '=')
		return (0);
	if (!(s[0] == '_' || ft_isalpha((unsigned char)s[0])))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(s[i] == '_' || ft_isalnum((unsigned char)s[i])))
			return (0);
		i++;
	}
	return (1);
}

int	env_set(char ***envp, const char *entry)
{
	int		idx;
	int		klen;
	char	**env;
	char	*neo;
	char	*oldv;

	if (!envp || !*envp || !entry)
		return (-1);
	env = *envp;
	klen = keylen(entry);
	idx = env_index_of(env, entry, klen);
	if (idx < 0)
		return (env_add((char *)entry, envp));
	neo = ft_strdup((char *)entry);
	if (!neo)
		return (-1);
	{
		oldv = env[idx];
		env[idx] = neo;
		free(oldv);
	}
	return (0);
}
