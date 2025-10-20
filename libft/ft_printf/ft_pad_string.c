/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pad_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken  <adjeuken@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 01:52:57 by adjeuken          #+#    #+#             */
/*   Updated: 2025/07/01 23:38:48 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	pad_negative_with_zeros(char *str, char *result, int padding)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	if (str[0] == '-')
		result[i++] = '-';
	if (str[0] == '+')
		result[i++] = '+';
	while (i < padding + 1)
		result[i++] = '0';
	while (str[j])
		result[i++] = str[j++];
	return (i);
}

static char	*ft_right_align(char *str, int n, char c, int length)
{
	int		i;
	int		j;
	char	*result;

	result = ft_calloc(n + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	if ((str[0] == '-' || str[0] == '+') && c == '0')
		i = pad_negative_with_zeros(str, result, n - length);
	else
	{
		if (length == 0)
			length = 1;
		while (i < n - length)
			result[i++] = c;
		while (i < n && str[j])
			result[i++] = str[j++];
	}
	result[n] = '\0';
	free(str);
	return (result);
}

static char	*ft_left_align(char *str, int n, char c)
{
	int		length;
	char	*result;
	int		i;

	length = ft_strlen(str);
	result = ft_calloc(n + 1, sizeof(char));
	i = 0;
	if (!result)
		return (NULL);
	if (length == 0)
		length = 1;
	while (i < length && i < n)
	{
		result[i] = str[i];
		i++;
	}
	while (i < n)
		result[i++] = c;
	result[n] = '\0';
	free(str);
	return (result);
}

/**
 * ft_pad_string - pads a string to a given width,
	either left- or right-aligned.

 * @str: The original string to pad. This string is freed i
 *         nside the called function.
 * @width: The total width of the resulting padded string.
 * @pad_char: The character used to pad the string.
 * @left_align: If true, pads on the right (left-aligns the string);
 *              if false, pads on the left (right-aligns the string).
 *
 * Returns: A newly allocated string of length @width with the original string
 *          padded and aligned according to @left_align.
 *          If allocation fails, returns NULL.
 *
 * Example:
 *     ft_pad_string(ft_strdup("Hi"), 5, '.', f_false) → "...Hi"
 *     ft_pad_string(ft_strdup("Hi"), 5, '.', f_true)  → "Hi..."
 */
char	*ft_pad_string(char *str, int width, char pad_char, t_bool left_align)
{
	int	len;

	if (left_align)
		return (ft_left_align(str, width, pad_char));
	else
	{
		len = ft_strlen(str);
		if (width < len)
			return (ft_right_align(str, len, pad_char, len));
		else
			return (ft_right_align(str, width, pad_char, len));
	}
}
