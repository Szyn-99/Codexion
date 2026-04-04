/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_croutine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:23:02 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/03 20:57:00 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	both_free(t_dongle *a, t_dongle *b, int id)
{
	long	now;

	now = get_time_ms();
	return (!a->in_use && !b->in_use
		&& a->queue.waiters[0].id == id
		&& b->queue.waiters[0].id == id
		&& now >= a->available_at
		&& now >= b->available_at);
}

static void	wait_for_pair(t_dongle *a, t_dongle *b, int id, t_codexion *cdx)
{
	while (!both_free(a, b, id) && !finished_simulation(cdx))
	{
		pthread_mutex_unlock(&b->dongle_mutex);
		pthread_mutex_unlock(&a->dongle_mutex);
		usleep(100);
		pthread_mutex_lock(&a->dongle_mutex);
		pthread_mutex_lock(&b->dongle_mutex);
	}
}

void	take_two_dongles(t_codexion *codex, t_coder *coder, int rd, int ld)
{
	t_dongle	*a;
	t_dongle	*b;
	int			got;
	long		now;

	a = &codex->dongles[(ld < rd) * ld + (ld >= rd) * rd];
	b = &codex->dongles[(ld < rd) * rd + (ld >= rd) * ld];
	pthread_mutex_lock(&a->dongle_mutex);
	pthread_mutex_lock(&b->dongle_mutex);
	push_coder(coder->id, choose_priority(coder), &a->queue);
	push_coder(coder->id, choose_priority(coder), &b->queue);
	wait_for_pair(a, b, coder->id, codex);
	got = !finished_simulation(codex);
	remove_coder(&a->queue);
	remove_coder(&b->queue);
	a->in_use = got;
	b->in_use = got;
	now = get_time_ms();
	pthread_mutex_unlock(&b->dongle_mutex);
	pthread_mutex_unlock(&a->dongle_mutex);
	if (got)
	{
		coder_logs(codex, now, coder->id, "has taken a dongle");
		coder_logs(codex, now, coder->id, "has taken a dongle");
	}
}

void	put_dongle(t_codexion *codex, int dongle_pos)
{
	t_dongle	*usb;

	usb = &codex->dongles[dongle_pos];
	pthread_mutex_lock(&usb->dongle_mutex);
	usb->in_use = 0;
	usb->available_at = get_time_ms() + codex->parse->dongle_cooldown;
	pthread_mutex_unlock(&usb->dongle_mutex);
}

void	coders_phases(t_coder *coder, int phase)
{
	long	ms;

	if (phase == 0x1)
	{
		pthread_mutex_lock(&coder->sim->sim_mutex);
		ms = get_time_ms();
		coder->last_compile_start = ms;
		coder->compiles++;
		pthread_mutex_unlock(&coder->sim->sim_mutex);
		coder_logs(coder->sim, ms, coder->id, "is compiling");
		usleep(coder->sim->parse->time_to_compile * 1000);
	}
	else if (phase == 0x2)
	{
		coder_logs(coder->sim, get_time_ms(), coder->id, "is debugging");
		usleep(coder->sim->parse->time_to_debug * 1000);
	}
	else if (phase == 0x3)
	{
		coder_logs(coder->sim, get_time_ms(), coder->id, "is refactoring");
		usleep(coder->sim->parse->time_to_refactor * 1000);
	}
}

static int	run_cycle(t_coder *coder, int rd, int ld)
{
	take_two_dongles(coder->sim, coder, rd, ld);
	if (finished_simulation(coder->sim))
	{
		put_dongle(coder->sim, ld);
		put_dongle(coder->sim, rd);
		return (0);
	}
	coders_phases(coder, 0x1);
	put_dongle(coder->sim, ld);
	put_dongle(coder->sim, rd);
	if (finished_simulation(coder->sim))
		return (0);
	coders_phases(coder, 0x2);
	if (finished_simulation(coder->sim))
		return (0);
	coders_phases(coder, 0x3);
	return (1);
}

void	*coders_routine(void *arg)
{
	t_coder	*coder;
	int		ld;
	int		rd;
	int		n;

	coder = (t_coder *)arg;
	n = coder->sim->parse->number_of_coders;
	if (n == 1)
	{
		while (!finished_simulation(coder->sim))
			usleep(1000);
		return (NULL);
	}
	ld = coder->id - 1;
	rd = coder->id % n;
	if (coder->id % 2 == 0)
		usleep((coder->sim->parse->time_to_compile
				+ coder->sim->parse->dongle_cooldown) * 1000);
	else if (n % 2 == 1 && coder->id == n)
		usleep(2 * (coder->sim->parse->time_to_compile
				+ coder->sim->parse->dongle_cooldown) * 1000);
	while (!finished_simulation(coder->sim))
		if (!run_cycle(coder, rd, ld))
			break ;
	return (NULL);
}
