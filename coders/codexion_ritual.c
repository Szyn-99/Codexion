/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_ritual.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:23:02 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/09 12:25:46 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_two_dongles(t_codexion *codex, t_coder *coder)
{
	int		got;
	long	now;

	pthread_mutex_lock(&coder->lowest_usb->usb_mutex);
	pthread_mutex_lock(&coder->highest_usb->usb_mutex);
	push_coder(coder->id, choose_priority(coder), &coder->lowest_usb->queue);
	push_coder(coder->id, choose_priority(coder), &coder->highest_usb->queue);
	bring_two_usbs(coder->lowest_usb, coder->highest_usb, coder->id, codex);
	got = !ritual_ended(codex);
	remove_coder(&coder->lowest_usb->queue);
	remove_coder(&coder->highest_usb->queue);
	coder->lowest_usb->free = got;
	coder->highest_usb->free = got;
	now = get_time_ms();
	pthread_mutex_unlock(&coder->highest_usb->usb_mutex);
	pthread_mutex_unlock(&coder->lowest_usb->usb_mutex);
	if (got)
	{
		coder_logs(codex, now, coder->id, "has taken a dongle");
		coder_logs(codex, now, coder->id, "has taken a dongle");
	}
	return (got);
}

void	put_dongle(t_usb *usb, t_codexion *codex)
{
	pthread_mutex_lock(&usb->usb_mutex);
	usb->free = 0;
	usb->served_at = get_time_ms() + codex->parse->dt_cooldown;
	pthread_cond_broadcast(&usb->usb_cond);
	pthread_mutex_unlock(&usb->usb_mutex);
}

void	coders_phases(t_coder *coder, int phase)
{
	long	ms;

	if (phase == 0x1)
	{
		pthread_mutex_lock(&coder->codexion->codex_mutex);
		ms = get_time_ms();
		coder->last_compile_start = ms;
		coder->compiles_done++;
		pthread_mutex_unlock(&coder->codexion->codex_mutex);
		coder_logs(coder->codexion, ms, coder->id, "is compiling");
		usleep(coder->codexion->parse->t_compile * 1000);
	}
	else if (phase == 0x2)
	{
		coder_logs(coder->codexion, get_time_ms(), coder->id, "is debugging");
		usleep(coder->codexion->parse->t_debug * 1000);
	}
	else if (phase == 0x3)
	{
		coder_logs(coder->codexion, get_time_ms(), coder->id, "is refactoring");
		usleep(coder->codexion->parse->t_refactor * 1000);
	}
}

static int	ritual(t_coder *coder)
{
	if (!take_two_dongles(coder->codexion, coder))
		return (0);
	coders_phases(coder, 0x1);
	put_dongle(coder->lowest_usb, coder->codexion);
	put_dongle(coder->highest_usb, coder->codexion);
	if (ritual_ended(coder->codexion))
		return (0);
	coders_phases(coder, 0x2);
	if (ritual_ended(coder->codexion))
		return (0);
	coders_phases(coder, 0x3);
	return (1);
}

void	*run_coders_ritual(void *arg)
{
	t_coder	*coder;
	int		n;

	coder = (t_coder *)arg;
	n = coder->codexion->parse->n_coders;
	if (n == 1)
	{
		while (!ritual_ended(coder->codexion))
			usleep(1000);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		usleep((coder->codexion->parse->t_compile
				+ coder->codexion->parse->dt_cooldown) * 1000);
	else if (n % 2 == 1 && coder->id == n)
		usleep(2 * (coder->codexion->parse->t_compile
				+ coder->codexion->parse->dt_cooldown) * 1000);
	while (!ritual_ended(coder->codexion))
		if (!ritual(coder))
			break ;
	return (NULL);
}
