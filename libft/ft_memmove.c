/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:12:21 by adjeuken          #+#    #+#             */
/*   Updated: 2025/05/21 09:20:37 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// ft_memmove copies 'n' bytes from memory area 'src' to memory area 'dest'.
// It handles overlapping memory regions safely by deciding whether to
// copy forwards or backwards.
// - If 'dest' and 'src' are the same or n is zero, no copying is done.
// - If 'dest' is before 'src', copying is done forward.
// - If 'dest' is after 'src',
//	copying is done backward to avoid overwriting data.

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d < s && n != 0)
	{
		i = 0;
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	if (d > s && n != 0)
	{
		i = n;
		while (i > 0)
		{
			i--;
			d[i] = s[i];
		}
	}
	return (dest);
}
