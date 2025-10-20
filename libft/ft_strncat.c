/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:50:45 by adjeuken          #+#    #+#             */
/*   Updated: 2025/05/21 11:18:16 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sft_strlen(char *str)
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

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	char	*start;
	size_t	i;
	size_t	l;

	start = dest;
	l = sft_strlen(dest);
	i = 0;
	while (src[i] && n > 0)
	{
		dest[l + i] = src[i];
		i++;
		n--;
	}
	dest[l + i] = '\0';
	return (start);
}
