/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_coders_ritual.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 23:39:06 by szyn              #+#    #+#             */
/*   Updated: 2026/04/09 11:54:16 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_ready(t_usb *d, int id)
{
	long	now;

	now = get_time_ms();
	return (!d->free && d->queue.waiters[0].id == id && now >= d->served_at);
}

void	wait_on_cond(t_usb *d, int id)
{
	struct timespec	ts;
	long			timeleft;

	if (!d->free && d->queue.waiters[0].id == id)
	{
		timeleft = d->served_at - get_time_ms();
		if (timeleft <= 0)
			return ;
		ts.tv_sec = d->served_at / 1000;
		ts.tv_nsec = (d->served_at % 1000) * 1000000;
		pthread_cond_timedwait(&d->usb_cond, &d->usb_mutex, &ts);
	}
	else
		pthread_cond_wait(&d->usb_cond, &d->usb_mutex);	
}

void	wait_for_pair(t_usb *a, t_usb *b, int id, t_codexion *cdx)
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
			pthread_mutex_unlock(&b->usb_mutex);
			wait_on_cond(a, id);
			pthread_mutex_lock(&b->usb_mutex);
		}
		else
		{
			pthread_mutex_unlock(&a->usb_mutex);
			wait_on_cond(b, id);
			pthread_mutex_unlock(&b->usb_mutex);
			pthread_mutex_lock(&a->usb_mutex);
			pthread_mutex_lock(&b->usb_mutex);
		}
	}
}
