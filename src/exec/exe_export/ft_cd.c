/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:37:18 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/16 12:36:19 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include <stdlib.h>

/* Join "PWD=" or "OLDPWD=" with current getcwd() */
static char	*cd_make_var(int which)
{
	char		*cwd;
	char		*res;
	const char	*prefix;
	size_t		plen;
	size_t		clen;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	if (which == 1)
		prefix = "PWD=";
	else
		prefix = "OLDPWD=";
	plen = ft_strlen(prefix);
	clen = ft_strlen(cwd);
	res = (char *)malloc(plen + clen + 1);
	if (!res)
		return ((free(cwd)), NULL);
	ft_memcpy(res, prefix, plen);
	ft_memcpy(res + plen, cwd, clen);
	res[plen + clen] = '\0';
	free(cwd);
	return (res);
}

/* Export two variables using your builtin `export` */
static int	cd_export_pair(const char *oldvar, const char *newvar, char ***env)
{
	if (env_set(env, oldvar) < 0)
		return (-1);
	if (env_set(env, newvar) < 0)
		return (-1);
	return (0);
}

/* helper: resolve target and perform chdir; prints errors as bash does */
static int	cd_chdir_resolved(char **argv, char **envp)
{
	const char	*target;

	if (!argv[1] || argv[1][0] == '\0')
	{
		target = env_get(envp, "HOME");
		if (!target || !target[0])
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return (1);
		}
		if (chdir(target) != 0)
			return (cd_perror(target));
		return (0);
	}
	if (ft_strcmp(argv[1], "-") == 0)
		return (cd_to_oldpwd(envp));
	target = argv[1];
	if (chdir(target) != 0)
		return (cd_perror(target));
	return (0);
}

/* public: builds OLDPWD before chdir, PWD after; updates env via export */
int	ft_cd(char **argv, char ***env, t_pool *pool)
{
	char	*oldvar;
	char	*newvar;
	int		rc;

	(void)pool;
	if (argv[1] && argv[2])
		return ((ft_dprintf(STDERR_FILENO,
					"minishell: cd: too many arguments\n")), 1);
	oldvar = cd_make_var(2);
	if (!oldvar)
		return (cd_perror(NULL));
	rc = cd_chdir_resolved(argv, *env);
	if (rc != 0)
		return ((free(oldvar)), rc);
	newvar = cd_make_var(1);
	if (!newvar)
		return ((free(oldvar)), cd_perror(NULL));
	if (cd_export_pair(oldvar, newvar, env) < 0)
	{
		ft_putstr_fd("minishell: cd: failed to update environment\n",
			STDERR_FILENO);
		return ((free(oldvar)), (free(newvar)), 1);
	}
	return ((free(oldvar)), (free(newvar)), 0);
}
