/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:06:33 by adjeuken          #+#    #+#             */
/*   Updated: 2025/05/26 18:02:50 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sft_strlen(const char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		count++;
		str++;
	}
	return (count);
}

static int	sft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	l;

	if ((!big || !little) && len == 0)
		return (NULL);
	if (!little[0])
		return ((char *)big);
	l = sft_strlen(little);
	i = 0;
	while (big[i] && i + l <= len)
	{
		if (sft_strncmp(&big[i], little, l) == 0)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
