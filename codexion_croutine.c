/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_croutine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:23:02 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/30 16:00:20 by aymel-ha         ###   ########.fr       */
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

void dongle(t_codexion *codex, t_coder *coder, int dongle_pos)
{
    pthread_mutex_lock(&codex->dongles[dongle_pos].dongle_mutex);
    if(!codex->dongles[dongle_pos].in_use && get_time_ms() >= codex->dongles[dongle_pos].available_at && !codex->dongles[dongle_pos].queue.size)
    {
        codex->dongles[dongle_pos].in_use = 1;
        pthread_mutex_unlock(&codex->dongles[dongle_pos].dongle_mutex);
        coder_logs(codex, get_time_ms(), coder->id, "has taken a dongle");
        return ;
    }
    push_coder(coder->id, choose_priority(coder), &codex->dongles[dongle_pos].queue);
    
}

void *coders_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    
    while(!coder->sim->sim_over)
    {
        
    }

    
}