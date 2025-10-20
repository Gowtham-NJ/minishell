/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:18:49 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:00:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	write_ptr_hex(char *dest, uintptr_t value, int len)
{
	int	i;
	int	digit;

	i = len - 1;
	while (i >= 2)
	{
		digit = value & 0xF;
		if (digit < 10)
			dest[i] = '0' + digit;
		else
			dest[i] = 'a' + (digit - 10);
		value >>= 4;
		i--;
	}
}

char	*ft_print_ptr_ill(t_flags *f)
{
	char	*nil_str;

	nil_str = ft_strdup("(nil)");
	if (!nil_str)
		return (NULL);
	if (f->width > 7)
		nil_str = ft_pad_string(nil_str, f->width, ' ', f->minus);
	return (nil_str);
}

char	*ft_print_ptr_and_return_len(t_flags *f, void *value)
{
	char		*result;
	uintptr_t	tmp;
	int			total_len;

	if (value == NULL)
		return (ft_print_ptr_ill(f));
	tmp = (uintptr_t)value;
	total_len = 1;
	while (tmp > 15)
	{
		tmp >>= 4;
		total_len++;
	}
	total_len += 2;
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	result[0] = '0';
	result[1] = 'x';
	write_ptr_hex(result, (uintptr_t)value, total_len);
	result[total_len] = '\0';
	if (f->width > total_len)
		result = ft_pad_string(result, f->width, ' ', f->minus);
	return (result);
}

int	ft_print_ptr(t_flags *f, va_list *args)
{
	char	*final_str;
	int		printed_len;

	final_str = ft_print_ptr_and_return_len(f, va_arg(*args, void *));
	if (!final_str)
		return (-1);
	printed_len = (int)write(f->fd, final_str, ft_strlen(final_str));
	free(final_str);
	return (printed_len);
}
