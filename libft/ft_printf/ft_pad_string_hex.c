/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pad_string_hex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 23:50:08 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/02 00:14:04 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_pad_temp(char *str, char pad, int t_len, t_bool align)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(t_len + 1);
	if (!result)
		return (NULL);
	if (align)
	{
		while (i < ft_strlen(str))
			result[i++] = str[j++];
		while (i < t_len)
			result[i++] = pad;
	}
	else
	{
		while (i < t_len - ft_strlen(str))
			result[i++] = pad;
		while (i < t_len)
			result[i++] = str[j++];
	}
	result[t_len] = '\0';
	return (result);
}

char	*unsigned_pad_string(char *str, int t_len, char pad, t_bool align)
{
	char	*result;

	if (!str || t_len < 0)
		return (NULL);
	if (t_len <= ft_strlen(str))
		return (str);
	result = ft_pad_temp(str, pad, t_len, align);
	free(str);
	return (result);
}

char	*temp_pad_string(const char *str, int t_len, char pad, t_bool align)
{
	char	*unsigned_part;
	char	*padded;
	char	sign;

	sign = 0;
	if (!str || t_len < 0)
		return (NULL);
	unsigned_part = ft_strdup((char *)str);
	if (str[0] == '-' || str[0] == '+')
	{
		sign = str[0];
		unsigned_part = ft_strdup((char *)(str + 1));
	}
	if (!unsigned_part)
		return (NULL);
	if (sign == 1)
	{
		padded = unsigned_pad_string(unsigned_part, t_len - 1, pad, align);
		padded = unsigned_pad_string(padded, t_len, sign, align);
	}
	else
		padded = unsigned_pad_string(unsigned_part, t_len, pad, align);
	return (padded);
}
