/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_env_copy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:12:43 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 19:53:59 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include "libft/libft.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void	ft_env_malloc_err(void)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

static void	ft_env_partial_free(char **copy)
{
	if (!copy)
		return ;
	ft_free_env_copy(copy);
}

static int	ft_make_env_copy_loop(char **envp, char ***copy, int i)
{
	(*copy)[i] = ft_strdup(envp[i]);
	if (!(*copy)[i])
	{
		ft_env_malloc_err();
		ft_env_partial_free(*copy);
		*copy = NULL;
		return (-1);
	}
	return (0);
}

/**
 * @brief Creates a copy of the env char **array. Allocates space for
 * it and fills it with data. A bigger array can be created so that
 * there is extra space to add something at the end manually afterwards.
 *
 * @param envp environment variable array gotten either from main or
 * a previous copy.
 * @param wanted_len if 0, it makes a clean copy, if num, it creates
 * a bigger array so that something can be added at the end. Has to
 * be based on the current env len(+1) -> it is the callers responsi-
 * bility. Otherwise the behaviour is undefined. Smaller array cannot
 * be created by default.
 */
char	**ft_make_env_copy(char **envp, size_t wanted_len)
{
	size_t	len;
	char	**copy;
	int		i;

	if (wanted_len == 0)
		len = ft_get_env_len(envp);
	else
		len = wanted_len;
	copy = (char **)malloc(sizeof(char *) * (len + 1));
	if (!copy)
	{
		ft_env_malloc_err();
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		if (ft_make_env_copy_loop(envp, &copy, i) != 0)
			return (NULL);
		i++;
	}
	if (!copy)
		return (NULL);
	copy[i] = NULL;
	return (copy);
}

// int main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	char **env_copy;

// 	env_copy = ft_make_env_copy(envp);
// 	int i = 0;
// 	while (env_copy[i])
// 	{
// 		printf("%s\n", env_copy[i]);
// 		i++;
// 	}
// }
