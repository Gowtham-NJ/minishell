/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:25:01 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/09 22:52:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	join_dir_cmd(char *dst, size_t cap, const char *dir,
		const char *cmd)
{
	size_t	ld0;
	size_t	ld;
	size_t	lc;
	size_t	total;

	ld0 = ft_strlen(dir);
	ld = ld0;
	lc = ft_strlen(cmd);
	if (ld == 0)
		ld = 1;
	total = ld + 1 + lc + 1;
	if (total > cap)
	{
		errno = ENAMETOOLONG;
		return (-1);
	}
	if (ld0 > 0)
		ft_memcpy(dst, dir, ld0);
	else
		dst[0] = '.';
	dst[ld] = '/';
	if (lc > 0)
		ft_memcpy(dst + ld + 1, cmd, lc);
	dst[ld + 1 + lc] = '\0';
	return (0);
}

static int	search_in_path(char *dst, size_t dstsz, const char *cmd,
		const char *path)
{
	size_t	i;
	size_t	j;
	char	dir[PATH_MAX];

	i = 0;
	while (path[i])
	{
		j = i;
		while (path[j] && path[j] != ':')
			j += 1;
		if (j - i < PATH_MAX)
		{
			ft_strlcpy(dir, path + i, (j - i) + 1);
			if (join_dir_cmd(dst, dstsz, dir, cmd) == 0 && access(dst,
					F_OK) == 0)
				return (0);
		}
		i = j + (path[j] != '\0');
	}
	errno = ENOENT;
	return (-1);
}

int	resolve_command(char *dst, size_t dstsz, const char *cmd,
		char *const envp[])
{
	const char	*path;
	int			rc;

	(void)envp;
	if (!cmd || !*cmd)
		return (-1);
	if (ft_strchr(cmd, '/') != NULL)
	{
		if ((size_t)ft_strlen(cmd) >= dstsz)
		{
			errno = ENAMETOOLONG;
			return (-1);
		}
		ft_strcpy(dst, cmd);
		return (0);
	}
	path = getenv("PATH");
	if (!path || !*path)
		path = "/usr/bin:/bin";
	rc = search_in_path(dst, dstsz, cmd, path);
	return (rc);
}
