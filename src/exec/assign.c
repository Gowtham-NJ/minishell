/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Codex                                         +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:00 by Codex             #+#    #+#             */
/*   Updated: 2025/10/11 21:10:00 by Codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <ctype.h>

int	is_assignment_word(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (s[i] == '=');
}

int	assign_prefix_len(char **argv)
{
	int	k;

	k = 0;
	while (argv && argv[k] && is_assignment_word(argv[k]))
		k++;
	return (k);
}
