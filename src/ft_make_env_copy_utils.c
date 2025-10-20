/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_env_copy_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:52:59 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/14 12:34:34 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include "libft/libft.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

size_t	ft_get_env_len(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}

void	ft_free_env_copy(char **env_copy)
{
	size_t	i;

	if (!env_copy)
		return ;
	i = 0;
	while (env_copy[i])
	{
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}
