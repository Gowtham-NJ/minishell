/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:19:03 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 12:31:30 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "executor.h"
#include "ft_export.h"
#include "status.h"
#include <stdlib.h>

/**
 * Has to do some error checking, then
 * print all that is in env, unordered,
 * skip stuff that has no "="
 * and return 0. Nothing crazy, really.
 */

void	env_print_skip_domain(char **env)
{
	size_t	i;

	i = 0;
	while (env && env[i])
	{
		if (!is_domain_entry(env[i]) && ft_strchr(env[i], '='))
		{
			write(1, env[i], ft_strlen(env[i]));
			write(1, "\n", 1);
		}
		i++;
	}
}

int	ft_env(char **argv, char **env)
{
	if (!argv || !argv[0])
		return (1);
	if (argv[1])
	{
		ft_putstr_fd("minishell: env: invalid usage\n", STDERR_FILENO);
		return (127);
	}
	if (!env)
		return (0);
	env_print_skip_domain(env);
	return (0);
}

char	**env_copy(char **envp)
{
	size_t	n;
	size_t	i;
	char	**neo;

	n = ft_get_env_len(envp);
	neo = (char **)malloc((n + 1) * sizeof(*neo));
	if (!neo)
		return (NULL);
	i = 0;
	while (i < n)
	{
		neo[i] = ft_strdup(envp[i]);
		if (!neo[i])
			return (NULL);
		i++;
	}
	neo[n] = NULL;
	return (neo);
}

int	env_add(char *entry, char ***envp)
{
	char	**old;
	char	**neo;
	size_t	n;
	size_t	i;

	if (!envp || !*envp || !entry)
		return (-1);
	old = *envp;
	n = ft_get_env_len(old);
	neo = (char **)malloc((n + 2) * sizeof(*neo));
	if (!neo)
		return (-1);
	i = 0;
	while (i < n)
	{
		neo[i] = old[i];
		i++;
	}
	neo[n] = ft_strdup(entry);
	if (!neo[n])
		return (-1);
	neo[n + 1] = NULL;
	*envp = neo;
	free(old);
	return (0);
}
