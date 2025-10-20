/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_long_long.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:13:48 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/09 14:50:14 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

long long	ft_atoi_long_long(const char *s)
{
	int			sign;
	long long	num;

	num = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		num = (num * 10) + (*s - '0');
		s++;
	}
	return (num * sign);
}
