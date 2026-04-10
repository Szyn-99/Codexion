/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:05:00 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/10 04:48:05 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	choose_priority(t_coder *coder)
{
	if (!coder->codexion->parse->scheduler[3])
		return (coder->last_compile_start + coder->codexion->parse->t_burnout);
	return (get_time_ms());
}

void	coder_logs(t_codexion *codex, long timestamp, int id, char *log)
{
	long	timeleft;

	timeleft = timestamp - codex->start_time;
	if (timeleft < 0)
		timeleft = 0;
	pthread_mutex_lock(&codex->log_mutex);
	if (!ritual_ended(codex) || log[0] == 'b')
		printf("%ld %d %s\n", timeleft, id, log);
	pthread_mutex_unlock(&codex->log_mutex);
}

int	ritual_ended(t_codexion *codex)
{
	int	finished;

	pthread_mutex_lock(&codex->codex_mutex);
	finished = codex->codex_over;
	pthread_mutex_unlock(&codex->codex_mutex);
	return (finished);
}

void	modify_ritual_status(t_codexion *codex, int status)
{
	pthread_mutex_lock(&codex->codex_mutex);
	codex->codex_over = status;
	pthread_mutex_unlock(&codex->codex_mutex);
}
