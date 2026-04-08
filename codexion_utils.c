/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:05:00 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/02 10:53:22 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;
	long			millisecond;

	gettimeofday(&tv, NULL);
	millisecond = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (millisecond);
}

long	choose_priority(t_coder *coder)
{
	if (!coder->codexion->parse->scheduler[3])
		return (coder->last_compile_start + coder->codexion->parse->t_burnout);
	return (get_time_ms());
}

void	coder_logs(t_codexion *codex, long timestamp, int id, char *log)
{
	long	elapsed;

	elapsed = timestamp - codex->start_time;
	if (elapsed < 0)
		elapsed = 0;
	pthread_mutex_lock(&codex->log_mutex);
	if (!finished_simulation(codex) || log[0] == 'b')
		printf("%ld %d %s\n", elapsed, id, log);
	pthread_mutex_unlock(&codex->log_mutex);
}

int	finished_simulation(t_codexion *codex)
{
	int	finished;

	pthread_mutex_lock(&codex->codex_mutex);
	finished = codex->codex_over;
	pthread_mutex_unlock(&codex->codex_mutex);
	return (finished);
}

void	modify_sim_status(t_codexion *codex, int status)
{
	pthread_mutex_lock(&codex->codex_mutex);
	codex->codex_over = status;
	pthread_mutex_unlock(&codex->codex_mutex);
}
