/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_uint.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:34:44 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:00:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_flag_u(t_flags *f, const char *num_part, int prec_zeros,
		int total_len)
{
	int		i;
	char	*final;
	char	*padded;

	final = malloc(total_len + 1);
	if (!final)
		return (NULL);
	i = 0;
	while (prec_zeros-- > 0)
		final[i++] = '0';
	ft_strlcpy(final + i, num_part, ft_strlen(num_part) + 1);
	if (f->width > total_len)
	{
		if (f->zero && !f->precision_specified && !f->minus)
			padded = temp_pad_string(final, f->width, '0', f->minus);
		else
			padded = temp_pad_string(final, f->width, ' ', f->minus);
		free(final);
		return (padded);
	}
	return (final);
}

char	*handle_zero_and_format_uint(t_flags *f, unsigned int value)
{
	char	*empty;
	char	*padded;

	if (value == 0 && f->precision_specified && f->precision == 0)
	{
		empty = ft_strdup("");
		if (!empty)
			return (NULL);
		if (f->width > 0)
		{
			padded = temp_pad_string(empty, f->width, ' ', f->minus);
			free(empty);
			return (padded);
		}
		return (empty);
	}
	return (NULL);
}

char	*ft_print_uint_and_return_len(t_flags *f, unsigned int value)
{
	char	*result;
	char	*num_part;
	int		len;
	int		prec_zeros;
	char	*final;

	result = handle_zero_and_format_uint(f, value);
	if (result)
		return (result);
	num_part = ft_itoa_uint_split(value);
	if (!num_part)
		return (NULL);
	len = ft_strlen(num_part);
	prec_zeros = 0;
	if (f->precision_specified && f->precision > len)
		prec_zeros = f->precision - len;
	final = ft_flag_u(f, num_part, prec_zeros, len + prec_zeros);
	free(num_part);
	return (final);
}

int	ft_print_uint(t_flags *f, va_list *args)
{
	char	*final_str;
	int		printed_len;

	final_str = ft_print_uint_and_return_len(f, va_arg(*args, unsigned int));
	if (!final_str)
		return (-1);
	printed_len = (int)write(f->fd, final_str, ft_strlen(final_str));
	free(final_str);
	return (printed_len);
}
