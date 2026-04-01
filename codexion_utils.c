/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:05:00 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/01 20:07:37 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
    long millisecond = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (millisecond);
}
long choose_priority(t_coder *coder)
{
    if (!coder->sim->parse->scheduler[3])
        return coder->last_compile_start + coder->sim->parse->time_to_burnout;
    return get_time_ms();
}

void coder_logs(t_codexion *codex, long timestamp, int id, char *log)
{
    long elapsed;

    elapsed = timestamp - codex->start;
    if (elapsed < 0)
        elapsed = 0;
    pthread_mutex_lock(&codex->log_mutex);
    printf("%ld %d %s\n", elapsed, id, log);
    pthread_mutex_unlock(&codex->log_mutex);
}

bool finished_simulation(t_codexion *codex)
{
    bool finished;
    pthread_mutex_lock(&codex->sim_mutex);
    finished = codex->sim_over;
    pthread_mutex_unlock(&codex->sim_mutex);
    return finished;
}

void modify_sim_status(t_codexion *codex, int status)
{
    pthread_mutex_lock(&codex->sim_mutex);
    codex->sim_over = status;
    pthread_mutex_unlock(&codex->sim_mutex);
}
