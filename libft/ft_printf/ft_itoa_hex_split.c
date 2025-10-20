/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_hex_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:46:38 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/02 01:45:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_count_hex_digits(unsigned int n)
{
	int	len;

	len = 1;
	while (n >= 16)
	{
		n /= 16;
		len++;
	}
	return (len);
}

char	*ft_itoa_hex_split(unsigned int n, char uppercase)
{
	int			len;
	const char	*base;
	char		*result;

	base = "0123456789abcdef";
	if (uppercase == 'X')
		base = "0123456789ABCDEF";
	len = ft_count_hex_digits(n);
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[len] = '\0';
	while (len--)
	{
		result[len] = base[n % 16];
		n /= 16;
	}
	return (result);
}
