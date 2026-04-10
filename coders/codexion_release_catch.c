/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_release_catch.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:30:19 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/10 10:07:20 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	release_coders(t_codexion *codex)
{
	int	i;

	codex->start_time = current_time_ms();
	i = 0;
	while (i < codex->parse->n_coders)
	{
		codex->coders[i].last_compile_start = codex->start_time;
		if (pthread_create(&codex->coders[i].thread, NULL, run_coders_ritual,
				&codex->coders[i]))
		{
			modify_ritual_status(codex, 1);
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
		modify_ritual_status(codex, 1);
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
}
