/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:47:33 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/14 16:39:56 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plan.h"

t_redir	*merge_redirs(t_redir *a, t_redir *b)
{
	t_redir	*tail;

	if (!b)
		return (a);
	tail = b;
	while (tail->next)
		tail = tail->next;
	tail->next = a;
	return (b);
}
