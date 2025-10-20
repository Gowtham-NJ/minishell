/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_percent.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:07:03 by adjeuken          #+#    #+#             */
/*   Updated: 2025/08/12 12:00:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_flag_percent_and_return_len(t_flags *f)
{
	char	*result;

	result = malloc(2);
	if (!result)
		return (NULL);
	result[0] = '%';
	result[1] = '\0';
	if (f->width > 1)
	{
		if (f->zero && !f->minus)
			result = ft_pad_string(result, f->width, '0', f->minus);
		else
			result = ft_pad_string(result, f->width, ' ', f->minus);
	}
	return (result);
}

int	ft_flag_percent(t_flags *f)
{
	char	*final_str;
	int		printed_len;

	final_str = ft_flag_percent_and_return_len(f);
	if (!final_str)
		return (-1);
	printed_len = (int)write(f->fd, final_str, ft_strlen(final_str));
	free(final_str);
	return (printed_len);
}
