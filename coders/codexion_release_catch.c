/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_release_catch.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:30:19 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/02 10:00:59 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	release_coders(t_codexion *codex)
{
	int	i;
	int	check;

	codex->start_time = get_time_ms();
	i = 0;
	while (i < codex->parse->n_coders)
	{
		codex->coders[i].last_compile_start = codex->start_time;
		check = pthread_create(&codex->coders[i].thread, NULL, coders_routine,
				&codex->coders[i]);
		if (check)
		{
			modify_sim_status(codex, 1);
			wake_coders(codex);
			return (i);
		}
		i++;
	}
	return (i);
}

bool	release_monitor(t_codexion *codex)
{
	if (pthread_create(&codex->monitor, NULL, monitor_over_coders, codex))
	{
		modify_sim_status(codex, 1);
		wake_coders(codex);
		return (false);
	}
	return (true);
}

void	catch_coders(t_codexion *codex, int succeeded)
{
	int	i;

	i = 0;
	while (i < succeeded)
	{
		pthread_join(codex->coders[i].thread, NULL);
		i++;
	}
	pthread_join(codex->monitor, NULL);
}
