/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_impl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:21:22 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/09 23:50:10 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	pwd_parse_args(char **argv, int *mode)
{
	int	i;

	*mode = 0;
	i = 1;
	while (argv[i] && argv[i][0] == '-')
	{
		if (ft_strcmp(argv[i], "--") == 0 || argv[i][1] == '\0')
			break ;
		if (ft_strcmp(argv[i], "-L") == 0)
			*mode = 0;
		else if (ft_strcmp(argv[i], "-P") == 0)
			*mode = 1;
		else
		{
			ft_dprintf(2, "pwd: %s: invalid option\n", argv[i]);
			ft_dprintf(2, "pwd: usage: pwd [-LP]\n");
			return (-2);
		}
		i++;
	}
	return (0);
}

int	bi_pwd(char **argv)
{
	int			mode;
	int			pr;
	char		*out;
	const char	*p;

	out = NULL;
	pr = pwd_parse_args(argv, &mode);
	if (pr == -2)
		return (2);
	if (pr != 0)
		return (pr);
	if (mode == 0)
	{
		p = getenv("PWD");
		if (p && p[0] == '/')
			out = ft_strdup((char *)p);
	}
	if (!out)
		out = getcwd(NULL, 0);
	if (!out)
		return ((ft_dprintf(2, "pwd: getcwd: %s\n", strerror(errno))), 1);
	ft_printf("%s\n", out);
	free(out);
	return (0);
}
