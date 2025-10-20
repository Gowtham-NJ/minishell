/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:21:14 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 12:28:17 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_unset.h"

/**
 * Small helper, just frees two strings and increments i by reference
 */
static void	ft_unset_delete_loop_helper(char *env, char *name, int *i)
{
	(void)env;
	(void)name;
	(*i)++;
}

/**
 * Delete loop. It gets the NAME part from each line, if it matches with
 * the arg during the loop, it frees the name, the one line in envp and
 * increments i - this happens in the above helper.
 * Else it just itterates over the array.
 *
 * Null terminates the new one.
 */
static void	ft_unset_delete_loop(t_pool *pool, char ***env, char ***copy,
		const char *arg)
{
	char	*name;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while ((*env)[i])
	{
		name = ft_unset_get_name(pool, (*env)[i]);
		if (!name)
		{
			ft_unset_free_copy(*copy);
			return ;
		}
		if (ft_strcmp(arg, name) == 0)
		{
			ft_unset_delete_loop_helper((*env)[i], name, &i);
			continue ;
		}
		(*copy)[j] = (*env)[i];
		i++;
		j++;
	}
	(*copy)[j] = NULL;
}

/**
 * Main part of the delete loop.
 * Gets the len of the current env arr,
 * allocates one less space,
 * then runs the actual delete loop.
 * Then it just swaps pointers and
 * frees the original arr.
 */
static void	ft_unset_delete(t_pool *pool, const char *arg, char ***env)
{
	char	**copy;
	char	**temp;
	int		len;

	len = ft_get_env_len(*env);
	copy = (char **)malloc(sizeof(char *) * (len - 1 + 1));
	if (!copy)
		return ;
	ft_unset_delete_loop(pool, env, &copy, arg);
	if (!copy)
		return ;
	temp = *env;
	*env = copy;
	free(temp);
}

/**
 * per argument check and execution
 *
 * Core functionality, checks for input errors, if there is one,
 * it returns one.
 *
 * Else it checks whether the specified argument is present in argv.
 * If it is, it runs ft_unset_delete, else if it is not, it just
 * returns 1. Could also return 0 as unset does not care about the
 * return value, but well...
 */
static int	ft_unset_check_n_delete(t_pool *pool, char ***env, const char *arg)
{
	if (ft_unset_input_check((char *)arg) == -1)
		return (-1);
	else
	{
		if (ft_unset_is_there(pool, arg, env) == 0)
			return (1);
		else
		{
			ft_unset_delete(pool, arg, env);
			return (0);
		}
	}
	return (0);
}

/**
 * if argument does not exist, return 0
 *
 * else, check the argument. As in export, process all of them in a loop,
 * check each one, if it has an error format, print an error and skip that one,
 * otherwise process and move to the next.
 *
 * This one, as in bash, returns 0 no matter what...
 */
int	ft_unset(char **argv, char ***env, t_pool *pool)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	else
	{
		while (argv[i])
		{
			ft_unset_check_n_delete(pool, env, argv[i]);
			i++;
		}
	}
	return (0);
}
