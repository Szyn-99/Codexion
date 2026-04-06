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

static int	check_ready(t_dongle *d, int id)
{
	long	now;

	now = get_time_ms();
	return (!d->in_use && d->queue.waiters[0].id == id
		&& now >= d->available_at);
}

static void	wait_on_cond(t_dongle *d, int id)
{
	struct timespec	ts;
	struct timeval	tv;
	long			wait_ms;
	long			now;

	now = get_time_ms();
	wait_ms = 5;
	if (!d->in_use && d->queue.waiters[0].id == id && now < d->available_at)
	{
		wait_ms = d->available_at - now;
		if (wait_ms > 10)
			wait_ms = 10;
		else if (wait_ms <= 0)
			wait_ms = 1;
	}
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (tv.tv_usec + wait_ms * 1000) / 1000000;
	ts.tv_nsec = ((tv.tv_usec + wait_ms * 1000) % 1000000) * 1000;
	pthread_cond_timedwait(&d->dongle_cond, &d->dongle_mutex, &ts);
}

static void	wait_for_pair(t_dongle *a, t_dongle *b, int id, t_codexion *cdx)
{
	int	a_r;
	int	b_r;

	while (!finished_simulation(cdx))
	{
		a_r = check_ready(a, id);
		b_r = check_ready(b, id);
		if (a_r && b_r)
			break ;
		if (!a_r)
		{
			pthread_mutex_unlock(&b->dongle_mutex);
			wait_on_cond(a, id);
			pthread_mutex_lock(&b->dongle_mutex);
		}
		else
		{
			pthread_mutex_unlock(&a->dongle_mutex);
			wait_on_cond(b, id);
			pthread_mutex_unlock(&b->dongle_mutex);
			pthread_mutex_lock(&a->dongle_mutex);
			pthread_mutex_lock(&b->dongle_mutex);
		}
	}
}

int	take_two_dongles(t_codexion *codex, t_coder *coder)
{
	t_dongle	*a;
	t_dongle	*b;
	int			got;
	long		now;
	
	a = coder->d1;
	b = coder->d2;
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
	return (got);
}

void	put_dongle(t_dongle *usb, t_codexion *codex)
{
	pthread_mutex_lock(&usb->dongle_mutex);
	usb->in_use = 0;
	usb->available_at = get_time_ms() + codex->parse->dongle_cooldown;
	pthread_cond_broadcast(&usb->dongle_cond);
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

static int	run_cycle(t_coder *coder)
{
	if (!take_two_dongles(coder->sim, coder))
		return (0);
	coders_phases(coder, 0x1);
	put_dongle(coder->d1, coder->sim);
	put_dongle(coder->d2, coder->sim);
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
	int		n;

	coder = (t_coder *)arg;
	n = coder->sim->parse->number_of_coders;
	if (n == 1)
	{
		while (!finished_simulation(coder->sim))
			usleep(1000);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		usleep((coder->sim->parse->time_to_compile
				+ coder->sim->parse->dongle_cooldown) * 1000);
	else if (n % 2 == 1 && coder->id == n)
		usleep(2 * (coder->sim->parse->time_to_compile
				+ coder->sim->parse->dongle_cooldown) * 1000);
	while (!finished_simulation(coder->sim))
		if (!run_cycle(coder))
			break ;
	return (NULL);
}
