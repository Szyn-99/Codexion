/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_croutine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:23:02 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/30 18:16:03 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
int ft_strlen(char *s)
{
    int i = 0;
    while(s[i])
        i++;
    return i;
}
void coder_logs(t_codexion *codex, long timestamp, int id, char *log)
{
    pthread_mutex_lock(&codex->log_mutex);
    printf("%ld %d %s", timestamp, id, log);
    pthread_mutex_unlock(&codex->log_mutex);
}

void compile_phase(t_coder *coder)
{
    
}
long choose_priority(t_coder *coder)
{
    if (ft_strlen(coder->sim->parse->scheduler) == 4)
        return get_time_ms();
    return coder->last_compile_start + coder->sim->parse->time_to_burnout;
}

void take_dongle(t_codexion *codex, t_coder *coder, int dongle_pos)
{
    t_dongle *usb = &codex->dongles[dongle_pos];
    struct timespec t;
    long timestamp;
    pthread_mutex_lock(&usb->dongle_mutex);
    push_coder(coder->id, choose_priority(coder), &usb->queue);
    while (usb->in_use || usb->queue.waiters[0].id != coder->id || get_time_ms() < usb->available_at)
    {
        if (!usb->in_use && usb->queue.waiters[0].id == coder->id && get_time_ms() < usb->available_at)
        {
            t.tv_sec = usb->available_at / 1000;
            t.tv_nsec = (usb->available_at % 1000) * 1000000;
            pthread_cond_timedwait(&usb->dongle_cond, &usb->dongle_mutex, &t);
        }
        else
            pthread_cond_wait(&usb->dongle_cond, &usb->dongle_mutex);
    }
    timestamp = get_time_ms();
    remove_coder(&usb->queue);
    usb->in_use = 1;
    pthread_mutex_unlock(&usb->dongle_mutex);
    coder_logs(codex, timestamp, coder->id, "has taken a dongle");
}

void take_two_dongles(t_codexion *codex, t_coder *coder, int rd, int ld)
{
    if (ld < rd)
    {
        take_dongle(codex, coder, ld);
        take_dongle(codex, coder, rd);
    }
    else
    {
        take_dongle(codex, coder, rd);
        take_dongle(codex, coder, ld);
    }
}
void put_dongle(t_codexion *codex, int dongle_pos)
{
    t_dongle *usb = &codex->dongles[dongle_pos];
    pthread_mutex_lock(&usb->dongle_mutex);
    usb->in_use = 0;
    usb->available_at = get_time_ms() + codex->parse->dongle_cooldown;
    pthread_mutex_unlock(&usb->dongle_mutex);
    pthread_cond_broadcast(&usb->dongle_cond);
}
void *coders_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    int ld, rd;
    ld = coder->id - 1;
    rd = coder->id % coder->sim->parse->number_of_coders;
    while(!coder->sim->sim_over)
    {
        take_two_dongles(coder->sim, coder, rd, ld);
    }

    
}