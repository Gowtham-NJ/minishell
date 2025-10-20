/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:49:07 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 16:26:13 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_export.h"

/**
 * helper - pointer swap for strings in array
 */
void	ft_export_swap_pointers(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * bubble sort for printing the env
 */
void	ft_export_sort_copy(char ***copy, int len)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		flag = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp((*copy)[j], (*copy)[j + 1]) > 0)
			{
				ft_export_swap_pointers(&(*copy)[j], &(*copy)[j + 1]);
				flag = 1;
			}
			j++;
		}
		if (flag == 0)
			break ;
		i++;
	}
}

/**
 * helper to free the malloced copy of envp
 */
void	ft_export_free_copy(char **copy)
{
	int	i;

	i = 0;
	while (copy[i])
	{
		free(copy[i]);
		i++;
	}
	free(copy);
}

/**
 * Helper function, so that everything is correctly
 * escaped, empty VAR= end up printed as VAR=""
 */
void	ft_export_print_loop(char *s)
{
	int	j;
	int	first;

	j = 0;
	first = 0;
	if (!ft_strchr(s, '='))
		return ((void)ft_printf("declare -x %s\n", s));
	ft_printf("declare -x ");
	while (s[j])
	{
		if (s[j] == '=' && s[j + 1] != '\0' && first == 0)
		{
			ft_printf("%c\"", s[j]);
			first = 1;
		}
		else if (s[j] == '=' && s[j + 1] == '\0' && first == 0)
			ft_printf("%c\"\"\n", s[j]);
		else if (s[j] && s[j + 1] == '\0')
			ft_printf("%c\"\n", s[j]);
		else
			ft_printf("%c", s[j]);
		j++;
	}
}

/**
 * Takes the whole argument as an argument, gets only the name
 * from it, allocates space for it and returns it.
 */
char	*ft_export_get_name(t_pool *pool, const char *arg)
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
