/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:50:08 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:14:16 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"
#include "parse_init.h"
#include "signal_setup.h"
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

static int	command_subst_span(t_exp_ctx *c, size_t *end)
{
	size_t	j;
	int		depth;
	int		quote;
	int		rc;

	j = c->i + 2;
	depth = 1;
	quote = 0;
	while (c->word[j])
	{
		rc = cs_span_step(c->word, &j, &depth, quote);
		if (rc != 0)
		{
			if (rc == 2 && depth == 0)
			{
				*end = j - 1;
				return (0);
			}
			continue ;
		}
		update_quote(c->word[j], &quote);
		j++;
	}
	return (-1);
}

static int	handle_command_subst(t_exp_ctx *c)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*body;
	char	*res;

	if (command_subst_span(c, &end) != 0)
		return (buf_putc(&c->out, c->word[c->i++]));
	start = c->i + 2;
	len = 0;
	if (end >= start)
		len = end - start;
	body = pool_strndup(c->exec_ctx->pool, c->word + start, len);
	if (!body)
		return (-1);
	res = command_subst_eval(c, body);
	if (!res)
		return (-1);
	if (res[0] && buf_putn(&c->out, res, ft_strlen(res)) != 0)
		return (-1);
	c->i = end + 1;
	return (0);
}

static int	handle_dollar(t_exp_ctx *c)
{
	size_t	j;
	char	*name;
	int		rc;

	if (c->quote == 1)
		return (buf_putc(&c->out, c->word[c->i++]));
	if (!c->word[c->i + 1])
		return (buf_putc(&c->out, c->word[c->i++]));
	if (c->word[c->i + 1] == '(')
		return (handle_command_subst(c));
	if (!(ft_isalpha((unsigned char)c->word[c->i + 1]) || c->word[c->i
				+ 1] == '_'))
		return (buf_putc(&c->out, c->word[c->i++]));
	c->i++;
	j = c->i;
	while (c->word[j] && (ft_isalnum((unsigned char)c->word[j])
			|| c->word[j] == '_'))
		j++;
	name = pool_strndup(c->exec_ctx->pool, c->word + c->i, j - c->i);
	if (!name)
		return (-1);
	rc = env_write_take(name, c->envp, &c->out);
	c->i = j;
	return (rc);
}

static int	expand_step_ctx(t_exp_ctx *c)
{
	if (c->word[c->i] == '\'' && c->quote != 2)
	{
		c->was_quoted = 1;
		if (c->quote == 1)
			c->quote = 0;
		else
			c->quote = 1;
		c->i++;
		return (0);
	}
	if (c->word[c->i] == '"' && c->quote != 1)
	{
		c->was_quoted = 1;
		if (c->quote == 2)
			c->quote = 0;
		else
			c->quote = 2;
		c->i++;
		return (0);
	}
	if (c->word[c->i] == '$')
		return (handle_dollar(c));
	if (buf_putc(&c->out, c->word[c->i++]) != 0)
		return (-1);
	return (0);
}

char	*expand_word_env(t_exec *exec_ctx, const char *word, char **envp,
		int *was_quoted)
{
	t_exp_ctx	c;

	c.exec_ctx = exec_ctx;
	if (buf_init(&c.out, c.exec_ctx->pool, 64) != 0)
		return (NULL);
	c.word = word;
	c.envp = envp;
	c.i = 0;
	c.quote = 0;
	c.was_quoted = 0;
	while (word[c.i])
	{
		if (expand_step_ctx(&c) != 0)
			return (NULL);
	}
	*was_quoted = c.was_quoted;
	return (buf_take(&c.out));
}
