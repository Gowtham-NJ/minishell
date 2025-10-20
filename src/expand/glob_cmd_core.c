/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_cmd_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:24:03 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:50:48 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "expand_internal.h"

/* Splice matches into argv at
 idx, sorting first; frees ms; returns 0 or -1. */
static int	splice_matches(t_cmd *cmd, int idx, char **ms, size_t n)
{
	size_t	k;

	if (n > 0)
	{
		strptr_insertion_sort(ms, n);
		if (replace_argv_segment(cmd, idx, ms, n) < 0)
			return (-1);
	}
	return (0);
}

/* Build dir/pattern, collect matches, delegate splice; always frees dir. */
static int	expand_word(t_cmd *cmd, int idx, const char *w)
{
	char		**ms;
	char		*dir;
	const char	*pat;
	ssize_t		k;
	int			r;

	r = build_dir_from_word(cmd->pool, w, &dir, &pat);
	if (r < 0)
		return (-1);
	if (r != 0 || *pat == '\0')
		return (0);
	k = collect_matches(cmd->pool, dir, pat, &ms);
	if (k < 0)
		return (-1);
	return (splice_matches(cmd, idx, ms, (size_t)k));
}

/* splice flags after expanding argv[i]:
replace one flag at i with (1+dn) zeros */
static int	splice_glob_flags(t_cmd *cmd, int i, size_t oldc, ssize_t dn)
{
	size_t			n1;
	size_t			nm;
	size_t			tail;
	unsigned char	*nf;

	if (!cmd->glob_ok)
		return (0);
	n1 = argv_count(cmd->argv);
	nm = (size_t)((ssize_t)1 + dn);
	nf = (unsigned char *)pool_malloc(cmd->pool, n1 * sizeof(unsigned char));
	if (!nf)
		return (-1);
	if (i > 0)
		ft_memcpy(nf, cmd->glob_ok, (size_t)i * sizeof(unsigned char));
	if (nm > 0)
		ft_memset(nf + i, 0, nm * sizeof(unsigned char));
	tail = oldc - (size_t)i - 1;
	if (tail > 0)
		ft_memcpy(nf + i + nm, cmd->glob_ok + i + 1, tail
			* sizeof(unsigned char));
	cmd->glob_ok = nf;
	return (0);
}

/* expand at index i, update i according
to dn (no ternary), and splice flags */
static int	expand_at_index(t_cmd *cmd, int *i)
{
	size_t	n0;
	size_t	n1;
	ssize_t	dn;

	n0 = argv_count(cmd->argv);
	if (expand_word(cmd, *i, cmd->argv[*i]) < 0)
		return (-1);
	n1 = argv_count(cmd->argv);
	dn = (ssize_t)n1 - (ssize_t)n0;
	if (splice_glob_flags(cmd, *i, n0, dn) < 0)
		return (-1);
	if (dn > 0)
		*i += (int)dn;
	else
		*i += 1;
	return (0);
}

int	expand_globs_in_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->argv)
		return (0);
	i = 0;
	while (cmd->argv[i])
	{
		if (is_assignment_word(cmd->argv[i]))
		{
			i += 1;
			continue ;
		}
		if (cmd->glob_ok && cmd->glob_ok[i])
		{
			if (expand_at_index(cmd, &i) < 0)
				return (-1);
		}
		else
			i += 1;
	}
	return (0);
}
