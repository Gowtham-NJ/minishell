/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:01:12 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 17:36:01 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include <stdlib.h>

/*
** heredoc_release: free every resource stored in a heredoc array.
** Inputs: pointer to the heredoc array and the number of entries.
** Outputs: none.
** Side effects: frees internal strings and the array allocation itself.
*/
void	heredoc_release(t_heredoc *docs, int count)
{
	int	index;

	if (!docs)
		return ;
	index = 0;
	while (index < count)
	{
		free(docs[index].body);
		free(docs[index].delimiter);
		free(docs[index].delimiter_raw);
		index += 1;
	}
	free(docs);
}
