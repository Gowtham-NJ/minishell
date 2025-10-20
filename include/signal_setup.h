/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_setup.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcerny <vcerny@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:38:09 by vcerny            #+#    #+#             */
/*   Updated: 2025/10/10 19:38:11 by vcerny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_SETUP_H
# define SIGNAL_SETUP_H

typedef enum e_sigflag	t_sigflag;

void	signal_prepare_prompt(void);
void	signal_prepare_noninteractive(void);
void	signal_prepare_child(void);
int		signal_flag_get(void);
void	signal_flag_set(t_sigflag flag);
void	signal_flag_clear(void);

#endif
