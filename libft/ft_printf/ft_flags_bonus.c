/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:28:22 by adjeuken          #+#    #+#             */
/*   Updated: 2025/06/29 16:07:25 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	normalize_flags(t_flags *f)
{
	if (f->plus || ft_strchr("uxX", f->specifier) || ft_strchr("csp%",
			f->specifier))
		f->space = t_false;
	if (f->minus)
		f->zero = t_false;
	if (f->precision_specified && ft_strchr("diuxX", f->specifier))
		f->zero = t_false;
	if (ft_strchr("uxX", f->specifier))
		f->plus = t_false;
	if (!ft_strchr("xX", f->specifier))
		f->hash = t_false;
	if (ft_strchr("csp%", f->specifier))
	{
		f->plus = t_false;
		f->zero = t_false;
	}
	if (f->precision_specified && !ft_strchr("diuxXs", f->specifier))
	{
		f->precision_specified = t_false;
		f->precision = 0;
	}
	if (ft_strchr("csp", f->specifier))
		f->zero = t_false;
}

t_flags	*ft_h_flags(t_flags *flags, const char *str)
{
	while (ft_isdigit(*str))
	{
		flags->width = flags->width * 10 + (*str - '0');
		str++;
	}
	if (*str == '.')
	{
		str++;
		flags->precision_specified = t_true;
		flags->precision = 0;
		while (ft_isdigit(*str))
		{
			flags->precision = flags->precision * 10 + (*str - '0');
			str++;
		}
	}
	if (ft_strchr("diuxXoscp%", *str))
		flags->specifier = *str;
	else
		flags->specifier = 0;
	normalize_flags(flags);
	return (flags);
}

t_flags	*ft_find_flags_id(const char *str)
{
	t_flags	*flags;

	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (NULL);
	*flags = (t_flags){0};
	if (*str == '%')
		str++;
	while (*str == '+' || *str == ' ' || *str == '-' || *str == '0'
		|| *str == '#')
	{
		if (*str == '+')
			flags->plus = t_true;
		else if (*str == ' ')
			flags->space = t_true;
		else if (*str == '-')
			flags->minus = t_true;
		else if (*str == '0')
			flags->zero = t_true;
		else if (*str == '#')
			flags->hash = t_true;
		str++;
	}
	return (ft_h_flags(flags, str));
}
