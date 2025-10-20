/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:07:48 by adjeuken          #+#    #+#             */
/*   Updated: 2025/05/21 09:57:57 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sft_isalpha(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	return ((uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z'));
}

static int	sft_isdigit(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	return (uc >= '0' && uc <= '9');
}

int	ft_isalnum(int c)
{
	return (sft_isalpha(c) || sft_isdigit(c));
}
