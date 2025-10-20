/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:12:02 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:23:53 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "minishell.h"
#include "proof.h"
#include "status.h"

static int	status_state_step(int state, char ch, int *handled)
{
	if (ch == '\'' && state != 2)
	{
		if (state == 1)
			state = 0;
		else
			state = 1;
		*handled = 1;
		return (state);
	}
	if (ch == '"' && state != 1)
	{
		if (state == 2)
			state = 0;
		else
			state = 2;
		*handled = 1;
		return (state);
	}
	*handled = 0;
	return (state);
}

static void	status_iter_step(t_status_iter *it, int *found, char *dst)
{
	int		handled;
	char	ch;

	ch = it->word[it->index];
	it->state = status_state_step(it->state, ch, &handled);
	if (ch == '$' && it->word[it->index + 1] == '?' && it->state != 1)
	{
		if (dst)
		{
			ft_memcpy(dst + it->write_pos, it->status, it->status_len);
			it->write_pos += it->status_len;
		}
		it->length += it->status_len;
		it->index += 2;
		if (found)
			*found = 1;
		return ;
	}
	it->length += 1;
	it->index += 1;
	if (dst)
	{
		dst[it->write_pos] = ch;
		it->write_pos += 1;
	}
}

static size_t	compute_new_length(const char *word, size_t status_len,
		int *found)
{
	t_status_iter	iter;

	iter.word = word;
	iter.status = NULL;
	iter.status_len = status_len;
	iter.index = 0;
	iter.length = 0;
	iter.write_pos = 0;
	iter.state = 0;
	*found = 0;
	while (word[iter.index] != '\0')
		status_iter_step(&iter, found, NULL);
	return (iter.length);
}

char	*status_replace_word_pool(t_pool *pool, const char *word,
		const char *status, size_t status_len)
{
	t_status_iter	iter;
	char			*result;
	size_t			new_len;
	int				found;

	new_len = compute_new_length(word, status_len, &found);
	if (found == 0)
		return (pool_strdup(pool, (char *)word));
	result = (char *)pool_calloc(pool, new_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	iter.word = word;
	iter.status = status;
	iter.status_len = status_len;
	iter.index = 0;
	iter.length = 0;
	iter.write_pos = 0;
	iter.state = 0;
	while (word[iter.index] != '\0')
		status_iter_step(&iter, NULL, result);
	return (result);
}

/*
** expand_exit_status: substitute `$?` tokens using the cached last status.
** Inputs: execution context, AST root.
** Outputs: returns 0 on success, -1 on allocation failure.
** Side effects: rewrites argv strings and redirection targets containing `$?`.
*/
int	expand_exit_status(t_exec *exec_ctx, t_ast *root)
{
	char	*status;
	size_t	status_len;
	int		result;

	status = ft_itoa(status_get(exec_ctx));
	if (!status)
		return (-1);
	status_len = ft_strlen(status);
	pool_track(exec_ctx->pool, status);
	result = status_apply_exit(exec_ctx->pool, root, status, status_len);
	return (result);
}
