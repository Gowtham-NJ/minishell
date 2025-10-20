/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_int_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:46:38 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/02 01:39:40 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	get_sign_char(int value, t_flags *f)
{
	if (value < 0)
		return ('-');
	if (f->plus)
		return ('+');
	if (f->space)
		return (' ');
	return ('\0');
}

static void	fill_number_digits(char *str, int n, int len)
{
	int	pos;
	int	digit;

	pos = len;
	if (n == 0)
	{
		str[--pos] = '0';
		return ;
	}
	while (n)
	{
		digit = n % 10;
		if (digit < 0)
			digit = -digit;
		str[--pos] = digit + '0';
		n /= 10;
	}
}

char	*ft_itoa_int(int n)
{
	int		len;
	char	*result;
	int		temp;

	temp = n;
	len = 0;
	if (temp == 0)
		len = 1;
	while (temp != 0)
	{
		len++;
		temp /= 10;
	}
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	result[len] = '\0';
	fill_number_digits(result, n, len);
	return (result);
}
