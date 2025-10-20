/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:51:51 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 12:29:59 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_export.h"

/**
 * Prints an error msg for an invalid name
 */
void	ft_export_print_error(t_pool *pool, const char *arg)
{
	const char	*name;
	char		*alloc;

	if (!arg)
		return ;
	alloc = ft_export_get_name(pool, arg);
	if (alloc)
		name = alloc;
	else
		name = arg;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)name, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
}

/**
 * Compares the current variable name with all names in the array
 * and returns 0 if it is not there, 1 if it is and -1 on error
 */
int	ft_export_is_there(t_pool *pool, const char *arg, char ***env)
{
	char	*name;
	char	*name_2;
	int		i;

	name = ft_export_get_name(pool, arg);
	if (!name)
		return (-1);
	i = 0;
	while ((*env)[i])
	{
		name_2 = ft_export_get_name(pool, (*env)[i]);
		if (ft_strcmp(name, name_2) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	export_err(const char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
}

int	is_domain_entry(const char *s)
{
	if (!s)
		return (0);
	if (s[0] != 'D' || s[1] != 'O' || s[2] != 'M' || s[3] != 'A' || s[4] != 'I'
		|| s[5] != 'N')
		return (0);
	return (s[6] == '\0' || s[6] == '=');
}
