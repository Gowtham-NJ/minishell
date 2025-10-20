/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:51:48 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 16:26:38 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_export.h"

static char	*next_min(const char *last, char **env)
{
	size_t	i;
	char	*best;

	i = 0;
	best = NULL;
	while (env && env[i])
	{
		if (!is_domain_entry(env[i]))
		{
			if ((!last || ft_strcmp(env[i], last) > 0) && (!best
					|| ft_strcmp(env[i], best) < 0))
				best = env[i];
		}
		i++;
	}
	return (best);
}

void	env_print_sorted_skip_domain(char **env)
{
	char	*last;
	char	*best;
	size_t	i;

	last = NULL;
	while (1)
	{
		best = next_min(last, env);
		if (!best)
			break ;
		i = 0;
		while (env && env[i])
		{
			if (ft_strcmp(env[i], best) == 0)
				ft_export_print_loop(env[i]);
			i++;
		}
		last = best;
	}
}

/* process one export argument:
   returns 0=ok, 1=invalid (error printed), -1=alloc/error */
/* returns: 0=ok, 1=invalid (error printed), -1=alloc/error */
static int	export_handle_arg(char ***envp, const char *arg)
{
	int	klen;
	int	idx;
	int	rc;

	if (!is_valid_ident(arg))
	{
		export_err(arg);
		return (1);
	}
	klen = keylen(arg);
	if (arg[klen] == '=')
	{
		rc = env_set(envp, arg);
		if (rc < 0)
			return (-1);
		return (0);
	}
	idx = env_index_of(*envp, arg, klen);
	if (idx < 0)
	{
		rc = env_set(envp, arg);
		if (rc < 0)
			return (-1);
	}
	return (0);
}

int	ft_export(char ***envp, char **argv)
{
	int	i;
	int	r;
	int	status;

	if (!argv || !argv[1])
	{
		env_print_sorted_skip_domain(*envp);
		return (0);
	}
	status = 0;
	i = 1;
	while (argv[i])
	{
		r = export_handle_arg(envp, argv[i]);
		if (r < 0)
			return (1);
		if (r > 0)
			status = 1;
		i++;
	}
	return (status);
}
