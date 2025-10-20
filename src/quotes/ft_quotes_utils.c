/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:04:29 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/15 18:33:50 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_quotes.h"

void	ft_quotes_write_error(void)
{
	ft_dprintf(STDERR_FILENO, "minishell: unexpected EOF while looking");
	ft_dprintf(STDERR_FILENO, " for matching `\"'\n");
}

int	ft_quotes_read_len(int read_fd, int *len)
{
	int	res;

	res = read(read_fd, len, sizeof(unsigned int));
	if (res != sizeof(unsigned int))
		return (-1);
	else if (errno == EINTR)
		return (-1);
	return (0);
}

int	ft_quotes_read_str(int read_fd, int len, char **str)
{
	int	res;

	res = read(read_fd, *str, len);
	if (res != len)
		return (-1);
	else if (errno == EINTR)
		return (-1);
	return (0);
}
