/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:51:21 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/02 10:53:00 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	detect_lazy_coder(t_codexion *codex)
{
	long	criteria;
	long	last;
	int		i;

	i = 0;
	while (i < codex->parse->number_of_coders)
	{
		criteria = get_time_ms();
		pthread_mutex_lock(&codex->sim_mutex);
		last = codex->coders[i].last_compile_start;
		pthread_mutex_unlock(&codex->sim_mutex);
		if (last + codex->parse->time_to_burnout < criteria)
			return (codex->coders[i].id);
		i++;
	}
	return (-42);
}

bool	is_done(t_codexion *codex)
{
	int	compiles;
	int	i;

	i = 0;
	while (i < codex->parse->number_of_coders)
	{
		pthread_mutex_lock(&codex->sim_mutex);
		compiles = codex->coders[i].compiles;
		pthread_mutex_unlock(&codex->sim_mutex);
		if (compiles < codex->parse->number_of_compiles_required)
			return (false);
		i++;
	}
	return (true);
}

void wake_coders(t_codexion *codex)
{
    int i = 0;
    while(i < codex->parse->number_of_dongles)
    {
        pthread_cond_broadcast(&codex->dongles[i].dongle_cond);
        i++;
    }
}
void	*monitor_over_coders(void *arg)
{
	t_codexion	*codex;
	int			lazy;

	codex = (t_codexion *)arg;
	while (!finished_simulation(codex))
	{
		lazy = detect_lazy_coder(codex);
		if (lazy != -42)
		{
			modify_sim_status(codex, 1);
			coder_logs(codex, get_time_ms(), lazy, "burned out");
			wake_coders(codex);
			return (NULL);
		}
		if (is_done(codex))
		{
			modify_sim_status(codex, 1);
			wake_coders(codex);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
