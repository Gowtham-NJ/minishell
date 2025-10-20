/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:27:22 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/05 20:27:44 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_unset.h"

/**
 * Helper
 * Actual rules for each char depending on position
 */
static int	ft_unset_is_allowed(char c, int i)
{
	if (c == '_' && i == 0)
		return (0);
	else if (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) && i == 0)
		return (0);
	else if (c == '_')
		return (0);
	else if (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) && i != 0)
		return (0);
	else if (c >= '0' && c <= '9' && i != 0)
		return (0);
	else
		return (-1);
}

/**
 * Checks the input argument against known rules.
 * If there is an error, it returns -1
 */
int	ft_unset_input_check(char *arg)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	if (arg[i] == '\0')
		error++;
	while (arg[i] && arg[i] != '=')
	{
		if (ft_unset_is_allowed(arg[i], i) == -1)
			error++;
		i++;
	}
	if (i == 0 && arg[i] == '=')
		error++;
	if (error > 0)
		return (-1);
	return (0);
}
