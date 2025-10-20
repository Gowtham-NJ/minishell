/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:01:51 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:00:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*hex_apply_precision_and_hash(char *num, t_flags *f)
{
	char	*prec;
	char	*prefixed;
	char	*tmp;

	if (f->precision_specified)
		prec = temp_pad_string(num, f->precision, '0', 0);
	else
		prec = ft_strdup(num);
	if (!prec)
		return (NULL);
	if (!(f->hash && ft_strcmp(prec, "0") != 0))
		return (prec);
	tmp = temp_pad_string(prec, ft_strlen(prec) + 1, f->specifier, t_false);
	if (!tmp)
	{
		free(prec);
		return (NULL);
	}
	prefixed = temp_pad_string(tmp, ft_strlen(prec) + 2, '0', t_false);
	free(prec);
	free(tmp);
	return (prefixed);
}

char	*ft_print_hex_h(t_flags *f, char *num)
{
	int		len;
	char	*result;

	len = ft_strlen(num);
	if (f->width <= len)
		return (ft_strdup(num));
	if (f->zero && !f->precision_specified && !f->minus)
		result = temp_pad_string(num, f->width, '0', t_false);
	else
		result = temp_pad_string(num, f->width, ' ', f->minus);
	return (result);
}

char	*ft_print_hex_and_return_len(t_flags *f, unsigned int value)
{
	char	*num;
	char	*tmp;
	char	*final;
	char	*empty;

	if (value == 0 && f->precision_specified && f->precision == 0)
	{
		empty = ft_strdup("");
		final = temp_pad_string(empty, f->width, ' ', f->minus);
		free(empty);
		return (final);
	}
	num = ft_itoa_hex_split(value, f->specifier);
	tmp = hex_apply_precision_and_hash(num, f);
	free(num);
	final = ft_print_hex_h(f, tmp);
	free(tmp);
	return (final);
}

int	ft_print_hex(t_flags *f, va_list *args)
{
	char	*final_str;
	int		printed_len;

	final_str = ft_print_hex_and_return_len(f, va_arg(*args, unsigned int));
	if (!final_str)
		return (-1);
	printed_len = (int)write(f->fd, final_str, ft_strlen(final_str));
	free(final_str);
	return (printed_len);
}
