/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:38:01 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/20 18:33:56 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	print_exec_error_and_code(const char *path, int err)
{
	const char	*msg;
	int			code;

	msg = strerror(err);
	code = 126;
	if (err == ENOENT)
	{
		msg = "No such file or directory";
		code = 127;
	}
	else if (err == ENOTDIR)
	{
		msg = "Not a directory";
		code = 127;
	}
	if (!path)
		path = "";
	ft_dprintf(2, "minishell: %s: %s\n", path, msg);
	return (code);
}

static int	exec_direct_or_report(const char *full, char **argv,
		char *const envp[])
{
	struct stat	st;
	int			rc;

	rc = stat(full, &st);
	if (rc == 0)
	{
		if (S_ISDIR(st.st_mode))
			return (print_exec_error_and_code(full, EISDIR));
	}
	execve(full, argv, envp);
	rc = errno;
	return (print_exec_error_and_code(full, rc));
}

int	exec_with_resolution(t_exec *E, char **argv)
{
	char		full[PATH_MAX];
	const char	*cmd;
	const char	*name;
	int			rc;

	cmd = NULL;
	name = "";
	if (argv && argv[0])
	{
		cmd = argv[0];
		name = argv[0];
	}
	if (!argv || !cmd || !*cmd)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", name);
		return (127);
	}
	if (ft_strchr(cmd, '/') != NULL)
		return (exec_direct_or_report(cmd, argv, *E->envp));
	rc = resolve_command(full, sizeof(full), cmd, *E->envp);
	if (rc == 0)
		return (exec_direct_or_report(full, argv, *E->envp));
	ft_dprintf(2, "minishell: %s: command not found\n", cmd);
	return (127);
}
