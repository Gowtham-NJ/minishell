/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjeuken <adjeuken@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:07:30 by adjeuken          #+#    #+#             */
/*   Updated: 2025/10/16 13:42:39 by adjeuken         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;
	int		status;

	(void)argc;
	(void)argv;
	if (ctx_begin(&ctx, envp) != 0)
		return (EXIT_FAILURE);
	status = shell_loop(&ctx);
	ctx_end(&ctx);
	return (status);
}
