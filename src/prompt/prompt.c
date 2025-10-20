/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:24:40 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/14 20:46:58 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/**
 * Reads hostname from the system, sets it in the env for
 * future calls and returns it
 */
static char	*ft_get_n_set_domain(char ***env)
{
	int		fd;
	char	c;
	int		len;
	char	*hostname;

	fd = open("/proc/sys/kernel/hostname", O_RDONLY);
	if (fd == -1)
		return (NULL);
	len = 0;
	while (read(fd, &c, 1) > 0)
	{
		len++;
	}
	hostname = (char *)malloc(sizeof(char) * (len));
	if (!hostname)
		return (NULL);
	close(fd);
	fd = open("/proc/sys/kernel/hostname", O_RDONLY);
	if (fd == -1)
		return (NULL);
	read(fd, hostname, len);
	hostname[len - 1] = '\0';
	ft_set_domain(hostname, env);
	close(fd);
	return (hostname);
}

// /* readline-safe ANSI colors */
// #define C0 "\001\033[0m\002"
// #define CW "\001\033[37m\002"  /* white  */
// #define CG "\001\033[32m\002"  /* green  */
// #define CB "\001\033[34m\002"  /* blue   */
/**
 * Combines all strings for the prompt together as well as adds supporting
 * chars
 */
char	*ft_combine_prompt(char *username, char *domain, char *pwd)
{
	char	*p;

	p = ft_strdup("\001\033[32m\002");
	if (!p || append(&p, username) < 0 || append(&p, "@") < 0 || append(&p,
			domain) < 0 || append(&p, "\001\033[0m\002") < 0 || append(&p,
			"\001\033[37m\002") < 0 || append(&p, ":") < 0 || append(&p,
			"\001\033[34m\002") < 0 || append(&p, pwd) < 0 || append(&p,
			"\001\033[0m\002") < 0 || append(&p, "\001\033[37m\002") < 0
		|| append(&p, "$ ") < 0 || append(&p, "\001\033[0m\002") < 0)
	{
		free(p);
		free(username);
		free(domain);
		free(pwd);
		return (NULL);
	}
	free(username);
	free(domain);
	free(pwd);
	return (p);
}

/**
 * Trimming function for pwd. Allocates space, writes ~ as the first
 * char and then copies from pwd to new after home len.
 */
static char	*ft_trim_pwd(char *pwd, char *home)
{
	char	*new;
	int		len;
	int		i;
	int		j;
	int		home_len;

	i = 1;
	j = 0;
	home_len = ft_strlen(home);
	len = ft_strlen(pwd) - home_len;
	new = (char *)malloc(sizeof(char) * (len + 2));
	if (!new)
		return (ft_strdup(pwd));
	new[0] = '~';
	while (pwd[j + home_len])
	{
		new[i] = pwd[j + home_len];
		i++;
		j++;
	}
	new[i] = '\0';
	return (new);
}

/**
 * Gets the full pwd and cleans it up.
 * If home is part of the complete pwd, it gets replaced by ~,
 * but the rest is left there. If home == pwd, it gets replaced by ~
 * entirely.
 * In all other cases, the complete pwd is returned
 */
static char	*ft_clean_up_pwd(char *pwd, char **env)
{
	char	*new_pwd;
	char	*home;

	home = ft_get_env_val(env, "HOME=");
	if (!home)
		return (pwd);
	if (ft_strlen(home) > 1 && home[ft_strlen(home) - 1] == '/')
		home[ft_strlen(home) - 1] = '\0';
	if (ft_strncmp(pwd, home, ft_strlen(home)) == 0 && pwd[ft_strlen(home)]
		&& pwd[ft_strlen(home)] == '/')
		new_pwd = ft_trim_pwd(pwd, home);
	else if (ft_strcmp(pwd, home) == 0)
		new_pwd = ft_strdup("~");
	else if (ft_strncmp(pwd, home, ft_strlen(home)) != 0)
		new_pwd = ft_strdup(pwd);
	else
		new_pwd = ft_strdup(pwd);
	free(home);
	free(pwd);
	return (new_pwd);
}

/**
 * Gets the info it needs for bash-like prompt creation,
 * combines all of the info together and returns a
 * finished prompt, or a prompt containing placeholders
 * in case of errors.
 */
char	*ft_make_prompt(char ***env)
{
	char	*username;
	char	*domain;
	char	*pwd;

	username = ft_get_env_val(*env, "USER=");
	if (!username)
		username = ft_strdup("no_username");
	pwd = ft_get_env_val(*env, "PWD=");
	if (!pwd)
		pwd = ft_strdup("no_pwd");
	domain = ft_get_env_val(*env, "DOMAIN=");
	if (!domain)
	{
		domain = ft_get_n_set_domain(env);
		if (!domain)
			domain = ft_strdup("no_domain");
	}
	pwd = ft_clean_up_pwd(pwd, *env);
	if (!pwd)
		pwd = ft_strdup("no_pwd");
	return (ft_combine_prompt(username, domain, pwd));
}

/**
 * the structure should be:
 * username@domain_name modified_pwd
 * klokanpes@pes:~/Desktop/minishell_builtins$
 *
 * strip pwd of first two entries
 */
// int	main(int argc, char **argv, char **envp)
// {
// 	char	**copy;
// 	char	*prompt;

// 	(void)argc;
// 	(void)argv;
// 	copy = ft_make_env_copy(envp, 0);
// 	prompt = ft_make_prompt(&copy);
// 	printf("%s\n", prompt);
// 	free(prompt);
// 	// int i = 0;
// 	// while (copy[i])
// 	// {
// 	// 	printf("%s\n", copy[i]);
// 	// 	i++;
// 	// }
// 	ft_free_env_copy(copy);
// }
