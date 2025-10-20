/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_long_long_check.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:26:59 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 19:30:11 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_atoi_long_long_check_helper(t_long *ret, const char s)
{
	if (!(ret->temp <= (unsigned long long)(LLMAX - (s - '0')) / 10)
		&& ret->sign == 1)
	{
		ret->flag = NOT_OK;
		return (-1);
	}
	else if (!(ret->temp <= (((unsigned long long)LLMAX + 1) - (s - '0')) / 10)
		&& ret->sign == -1)
	{
		ret->flag = NOT_OK;
		return (-1);
	}
	ret->temp = (ret->temp * 10) + (s - '0');
	return (0);
}

t_long	ft_atoi_long_long_check(const char *s)
{
	t_long	ret;

	ret.temp = 0;
	ret.sign = 1;
	ret.flag = YES_OK;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			ret.sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		if (ft_atoi_long_long_check_helper(&ret, *s) == -1)
			break ;
		s++;
	}
	ret.num = 0;
	return (ret);
}
