/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:30:45 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/14 22:54:44 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proof.h"
#include "status.h"
#include <signal.h>
#include <sys/wait.h>

int	status_get(t_exec *exec_ctx)
{
	int	value;

	if (!exec_ctx)
		return (0);
	value = exec_ctx->last_status;
	return (value & 0xFF);
}

void	status_set(t_exec *exec_ctx, int status)
{
	int	trimmed;

	if (!exec_ctx)
		return ;
	trimmed = status & 0xFF;
	exec_ctx->last_status = trimmed;
}

/**
 * Added bash like signal handlers, so that if we detect WTERMSIG,
 * we print Quit,  if we detect a core dump, we print (core dumped).
 * Else we print only a new_line.  The carrets ^\ && ^C get printed by
 * ECHOCTL in this case.  But bash does the same.
 * Since there are no custom signal handlers in the children,
 * this has to be like that.
 * The input for heredocs and continuous lines is different so it is hardcoded,
 *  as well as input in the parent.
 */
int	status_from_wait(int wait_status)
{
	int	code;

	code = 1;
	if (WIFEXITED(wait_status))
		code = WEXITSTATUS(wait_status);
	else if (WIFSIGNALED(wait_status))
	{
		code = 128 + WTERMSIG(wait_status);
		if (WTERMSIG(wait_status) == SIGQUIT)
			printf("Quit");
		if (__WCOREDUMP(wait_status))
			printf(" (core dumped)");
		printf("\n");
	}
	return (code & 0xFF);
}
