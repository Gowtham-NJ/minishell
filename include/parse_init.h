/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:37 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/15 15:30:57 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "parse.h"
#include "tok.h"
#include <stddef.h>
#include "libft/libft.h"

typedef struct s_simple_parts
{
	char	**argv;
	size_t	argc;
	size_t	cap;
	t_redir	*redirs;
	t_redir	**tail;
}			t_simple_parts;

/* ---- core helpers (now non-static, prefixed) ---- */
t_token		*p_cur(t_parser *P);
int			p_accept(t_parser *P, t_tok_kind k);
void		p_set_error(t_parser *P, const char *msg, const char *tok);
char		*p_sndup(t_parser *P, const char *s, size_t n);

/* ---- redirection helpers ---- */
int			is_redir_token(t_tok_kind k);
char		*parse_redir_target(t_parser *P);
t_redir		*redir_from_kind(t_pool *pool, t_tok_kind kind, const char *target);
t_redir		*parse_redirection(t_parser *P);
void		free_redirs_temp(t_redir *r);

/* ---- argv/simple-command helpers ---- */
int			append_word(t_parser *P, char ***argv, size_t *argc, size_t *cap);
void		free_argv_temp(char **argv, size_t count);
void		init_simple_parts(t_simple_parts *p);
int			collect_fail(t_simple_parts *p);
int			parse_simple_command_helper(t_parser *P, t_simple_parts *parts);

/* ---- node builders ---- */
t_ast		*parse_simple_command(t_parser *P);
int			parse_group_redirs(t_parser *P, t_redir **out_head);
t_ast		*parse_group(t_parser *P);
t_ast		*parse_command(t_parser *P);
t_ast		*parse_pipeline(t_parser *P);
t_ast		*parse_and_or(t_parser *P);
t_ast		*parse_list(t_parser *P);
