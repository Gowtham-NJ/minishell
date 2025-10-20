/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:39:26 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 19:39:28 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_H
# define STATUS_H

typedef struct s_exec	t_exec;

int						status_get(t_exec *exec_ctx);
void					status_set(t_exec *exec_ctx, int status);
int						status_from_wait(int wait_status);

#endif
