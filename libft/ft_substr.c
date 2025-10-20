/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:05:47 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/04 10:40:55 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy_substr(const char *s, unsigned int start,
		size_t copy_len)
{
	char	*substr;
	size_t	i;

	substr = malloc(copy_len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < copy_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	str_len;
	size_t	copy_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strncpy_substr("", 0, 0));
	if (str_len - start < len)
		copy_len = str_len - start;
	else
		copy_len = len;
	return (ft_strncpy_substr(s, start, copy_len));
}
