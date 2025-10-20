/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:45:30 by adjeuken          #+#    #+#             */
/*   Updated: 2025/05/26 16:54:58 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_alloc_str(size_t n, size_t ls1, char const *s1)
{
	char	*str;
	size_t	i;

	str = malloc(n * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < ls1)
	{
		str[i] = s1[i];
		i++;
	}
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	ls1;
	size_t	ls2;
	size_t	i;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	str = ft_alloc_str(ls1 + ls2 + 1, ls1, s1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < ls2)
	{
		str[ls1 + i] = s2[i];
		i++;
	}
	str[ls1 + ls2] = '\0';
	return (str);
}
