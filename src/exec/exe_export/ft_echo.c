/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:17:49 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/14 15:24:42 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "executor.h"
#include "ft_export.h"
#include "status.h"

static int	ft_echo_is_minus_n(char *s)
{
	int	i;

	i = 1;
	if (!s[0] || s[0] != '-' || !s[1])
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * sets the suppress variable to no, then checks basic input, if there
 * is an error, returns error immediately. If there is no argv[1], it just
 * prints a new line and exits with 0. Then it checks whether argv[1]
 * matches all the possible -n variations (-n, -nn, -nnn, ...). If so, it
 * sets the suppress var to YES and advances i.
 * Then, all following nodes that only contain variations of -n are skipped.
 * This is in my opinion stupid behaviour but it is what bash does.
 * Then, once one does not match with this rule it prints all others,
 * putting one space between them. except the last one does not get a
 * space.
 *
 * Lastly, if the suppress_n still has NO_SUPPRESS value, meaning
 * there was no -n node, it prints a new line and exits with 0.
 */
int	ft_echo(char **argv)
{
	int	i;
	int	suppress_nl;

	i = 1;
	if (!argv || !argv[0])
		return (1);
	if (!argv[1])
		return ((ft_printf("\n")), 0);
	if (ft_echo_is_minus_n(argv[1]))
		i++;
	suppress_nl = i - 1;
	while (argv[i] && ft_echo_is_minus_n(argv[i]))
		i++;
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!suppress_nl)
		ft_printf("\n");
	return (0);
}
