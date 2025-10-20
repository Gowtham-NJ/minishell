/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:18:57 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:07:13 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_handle_c(char value, t_flags *f)
{
	int		padding;
	char	pad_char;
	int		width;

	width = 1;
	pad_char = ' ';
	if (f->width > 1)
		width = f->width;
	padding = width - 1;
	if (f->zero && !f->minus)
		pad_char = '0';
	if (!f->minus)
	{
		ft_putnchr(pad_char, padding, f->fd);
		write(f->fd, &value, 1);
	}
	else
	{
		write(f->fd, &value, 1);
		ft_putnchr(pad_char, padding, f->fd);
	}
	return (width);
}

static char	*handle_null_str_with_precision(const char *s,
		int precision_specified, int precision)
{
	int		len;
	char	*res;

	if (s == NULL && precision_specified && precision < 6)
		return (ft_strdup(""));
	if (s == NULL && precision_specified && precision >= 6)
		precision = 6;
	if (s == NULL && !precision_specified)
		return (ft_strdup("(null)"));
	if (s == NULL)
		s = "(null)";
	if (precision_specified && precision >= 0)
	{
		len = ft_strlen(s);
		if (precision < len)
			len = precision;
		res = malloc(len + 1);
		if (!res)
			return (NULL);
		ft_memcpy(res, s, len);
		res[len] = '\0';
		return (res);
	}
	else
		return (ft_strdup((char *)s));
}

int	ft_handle_s(char *s, t_flags *f)
{
	char	*str;
	int		printed_chars;
	char	pad_char;
	int		len;
	int		padding;

	printed_chars = 0;
	pad_char = ' ';
	str = handle_null_str_with_precision(s, f->precision_specified,
			f->precision);
	if (!str)
		return (-1);
	len = (int)ft_strlen(str);
	padding = 0;
	if (f->width > len)
		padding = f->width - len;
	if (!f->minus)
		printed_chars += ft_putnchr(pad_char, padding, f->fd);
	printed_chars += write(f->fd, str, len);
	if (f->minus)
		printed_chars += ft_putnchr(pad_char, padding, f->fd);
	free(str);
	return (printed_chars);
}

int	ft_format_str_or_char(t_flags *f, va_list *args)
{
	if (f->specifier == 'c')
		return (ft_handle_c(va_arg(*args, int), f));
	else if (f->specifier == 's')
		return (ft_handle_s(va_arg(*args, char *), f));
	return (0);
}
