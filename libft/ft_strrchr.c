/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:03:51 by adjeuken          #+#    #+#             */
/*   Updated: 2025/05/26 13:39:37 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char			*r;
	unsigned char	uc;

	uc = (unsigned char)c;
	r = NULL;
	while (*s)
	{
		if ((unsigned char)*s == uc)
			r = (char *)s;
		s++;
	}
	if ((unsigned char)*s == uc)
		r = (char *)s;
	return (r);
}
