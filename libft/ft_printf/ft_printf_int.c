/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_int.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:51:26 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:00:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*build_number_str(char *num_part, int value, int prec_zeros,
		t_flags *f)
{
	char	sign_char;
	int		num_len;
	int		total_len;
	char	*final;
	char	*p;

	sign_char = get_sign_char(value, f);
	num_len = ft_strlen(num_part);
	total_len = num_len + prec_zeros;
	if (sign_char)
		total_len = total_len + 1;
	final = malloc(total_len + 1);
	if (!final)
		return (NULL);
	p = final;
	if (sign_char)
		*p++ = sign_char;
	while (prec_zeros-- > 0)
		*p++ = '0';
	ft_strlcpy(p, num_part, num_len + 1);
	return (final);
}

char	*handle_zero_precision_and_width(t_flags *f, int value, char *num_part)
{
	char	*empty;
	char	*padded;
	char	*final;

	final = NULL;
	final = NULL;
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
	if (f->precision_specified && f->precision > ft_strlen(num_part))
		final = build_number_str(num_part, value, f->precision
				- ft_strlen(num_part), f);
	else
		final = build_number_str(num_part, value, 0, f);
	return (final);
}

char	*ft_print_int_and_return_len(t_flags *f, int value)
{
	char	*num_part;
	char	*final;

	num_part = ft_itoa_int(value);
	if (!num_part)
		return (NULL);
	final = handle_zero_precision_and_width(f, value, num_part);
	free(num_part);
	if (!final)
		return (NULL);
	if (f->width > (int)ft_strlen(final) && f->zero && !f->precision_specified
		&& !f->minus)
		final = ft_pad_string(final, f->width, '0', f->minus);
	else if (f->width > (int)ft_strlen(final))
		final = ft_pad_string(final, f->width, ' ', f->minus);
	return (final);
}

int	ft_print_int(t_flags *f, va_list *args)
{
	char	*final_str;
	int		printed_len;

	final_str = ft_print_int_and_return_len(f, va_arg(*args, int));
	if (!final_str)
		return (-1);
	printed_len = (int)write(f->fd, final_str, ft_strlen(final_str));
	free(final_str);
	return (printed_len);
}
