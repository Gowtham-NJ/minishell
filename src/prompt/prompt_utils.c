/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:23:30 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/14 18:30:57 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/**
 * Helper for get_env_val
 */
static void	ft_get_env_val_helper(char **env, int len, int i, char **env_val)
{
	int	j;

	j = 0;
	while (env[i][j + len])
	{
		(*env_val)[j] = env[i][j + len];
		j++;
	}
	(*env_val)[j] = '\0';
}

/**
 * gets the value of a variable from env. Currently used to get
 * the path to HOME and OLDPWD.
 */
char	*ft_get_env_val(char **env, char *name)
{
	int		i;
	int		name_len;
	int		len;
	char	*env_val;

	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0)
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	len = ft_strlen(env[i]) - name_len;
	env_val = (char *)malloc(sizeof(char) * (len + 1));
	if (!env_val)
		return (NULL);
	ft_get_env_val_helper(env, name_len, i, &env_val);
	return (env_val);
}

/**
 * Sets the hostname(domain) in ENV for future calls
 * using ft_export
 */
void	ft_set_domain(char *hostname, char ***env)
{
	char	**argv;
	char	*new_hostname;

	new_hostname = ft_strjoin("DOMAIN=", hostname);
	argv = (char **)malloc(sizeof(char *) * 3);
	argv[0] = ft_strdup("export");
	argv[1] = new_hostname;
	argv[2] = NULL;
	ft_export(env, argv);
	free(argv[0]);
	free(argv);
	free(new_hostname);
}

int	append(char **dst, const char *s)
{
	char	*old;
	char	*nw;

	old = *dst;
	nw = ft_strjoin(old, s);
	if (!nw)
		return (-1);
	free(old);
	*dst = nw;
	return (0);
}
