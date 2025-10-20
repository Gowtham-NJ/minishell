/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:28:26 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 12:28:47 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_unset.h"

/**
 * Takes the whole argument as an argument, gets only the name
 * from it, allocates space for it and returns it.
 */
char	*ft_unset_get_name(t_pool *pool, const char *arg)
{
	int		len;
	char	*name;
	int		i;

	len = 0;
	i = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	name = (char *)pool_malloc(pool, (len + 1));
	if (!name)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (NULL);
	}
	while (i < len)
	{
		name[i] = arg[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

/**
 * Compares the current variable name with all names in the array
 * and returns 0 if it is not there, 1 if it is and -1 on error
 */
int	ft_unset_is_there(t_pool *pool, const char *arg, char ***env)
{
	char	*name_2;
	int		i;

	i = 0;
	while ((*env)[i])
	{
		name_2 = ft_unset_get_name(pool, (*env)[i]);
		if (!name_2)
			return (-1);
		if (ft_strcmp(arg, name_2) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * helper to free the malloced copy of envp
 */
void	ft_unset_free_copy(char **copy)
{
	(void)copy;
}
