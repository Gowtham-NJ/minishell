/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:37:18 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 01:19:49 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

const char	*env_get(char **env, const char *key)
{
	int		i;
	size_t	klen;

	klen = ft_strlen(key);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, klen) && env[i][klen] == '=')
			return (env[i] + klen + 1);
		i++;
	}
	return (NULL);
}

int	cd_perror(const char *arg)
{
	ft_dprintf(STDERR_FILENO, "minishell: cd ");
	if (arg)
		ft_dprintf(STDERR_FILENO, " %s: ", arg);
	ft_dprintf(STDERR_FILENO, "%s\n", strerror(errno));
	return (1);
}

/* cd - : chdir to $OLDPWD and print it */
int	cd_to_oldpwd(char **env)
{
	char		*now;
	const char	*old;

	old = env_get(env, "OLDPWD");
	if (!old)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
	if (chdir(old) != 0)
		return (cd_perror(old));
	now = getcwd(NULL, 0);
	if (now)
	{
		ft_putendl_fd(now, STDOUT_FILENO);
		free(now);
	}
	return (0);
}
