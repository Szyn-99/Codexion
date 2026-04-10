/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:51:21 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/10 10:17:35 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	detect_lazy_coder(t_codexion *codex)
{
	long	last;
	long	criteria;
	int		i;

	i = 0;
	while (i < codex->parse->n_coders)
	{
		criteria = current_time_ms();
		pthread_mutex_lock(&codex->codex_mutex);
		last = codex->coders[i].last_compile_start;
		pthread_mutex_unlock(&codex->codex_mutex);
		if (last + codex->parse->t_burnout < criteria)
			return (codex->coders[i].id);
		i++;
	}
	return (-42);
}

bool	goal_achieved(t_codexion *codex)
{
	int	compiles;
	int	i;

	i = 0;
	while (i < codex->parse->n_coders)
	{
		pthread_mutex_lock(&codex->codex_mutex);
		compiles = codex->coders[i].compiles_done;
		pthread_mutex_unlock(&codex->codex_mutex);
		if (compiles < codex->parse->n_compiles)
			return (false);
		i++;
	}
	return (true);
}

void	wake_coders(t_codexion *codex)
{
	int	i;

	i = 0;
	while (i < codex->parse->n_dongles)
	{
		pthread_mutex_lock(&codex->usbs[i].usb_mutex);
		pthread_cond_broadcast(&codex->usbs[i].usb_cond);
		pthread_mutex_unlock(&codex->usbs[i].usb_mutex);
		i++;
	}
}

void	*monitor_over_coders(void *arg)
{
	t_codexion	*codex;
	int			lazy;

	codex = (t_codexion *)arg;
	while (!ritual_ended(codex))
	{
		lazy = detect_lazy_coder(codex);
		if (lazy != -42)
		{
			modify_ritual_status(codex, 1);
			coder_logs(codex, current_time_ms(), lazy, "burned out");
			wake_coders(codex);
			return (NULL);
		}
		if (goal_achieved(codex))
		{
			modify_ritual_status(codex, 1);
			wake_coders(codex);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
