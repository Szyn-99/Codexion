/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_coders_ritual.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 23:39:06 by szyn              #+#    #+#             */
/*   Updated: 2026/04/10 04:09:59 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	usb_ready(t_usb *d, int id)
{
	return (!d->free && d->queue.waiters[0].id == id
		&& get_time_ms() >= d->served_at);
}

void	wait_for_usb(t_usb *d, int id)
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

void	bring_two_usbs(t_usb *lowest, t_usb *highest, int id, t_codexion *cdx)
{
	int	lowest_r;
	int	highest_r;

	while (!ritual_ended(cdx))
	{
		lowest_r = usb_ready(lowest, id);
		highest_r = usb_ready(highest, id);
		if (lowest_r && highest_r)
			break ;
		if (!lowest_r)
		{
			pthread_mutex_unlock(&highest->usb_mutex);
			wait_for_usb(lowest, id);
			pthread_mutex_lock(&highest->usb_mutex);
		}
		else
		{
			pthread_mutex_unlock(&lowest->usb_mutex);
			wait_for_usb(highest, id);
			pthread_mutex_unlock(&highest->usb_mutex);
			pthread_mutex_lock(&lowest->usb_mutex);
			pthread_mutex_lock(&highest->usb_mutex);
		}
	}
}
