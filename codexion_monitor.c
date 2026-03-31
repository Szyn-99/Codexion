/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:51:21 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/31 14:18:28 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int detect_lazy_coder(t_coder *coders)
{
    long criteria = get_time_ms();
    int i = 0;
    while(i < coders->sim->parse->number_of_coders)
    {
        if (coders[i].last_compile_start + coders->sim->parse->time_to_burnout <= criteria)
            return coders[i].id;
        i++;
    }
    return -42;
}
bool is_done(t_coder *coders)
{
    int i = 0;
    while (i < coders->sim->parse->number_of_coders)
    {
        if(coders[i].compiles != coders->sim->parse->number_of_compiles_required)
            return false;
    }
    return true;
}

void wake_coders(t_codexion *codex)
{
    int i = 0;
    while(i < codex->parse->number_of_dongles)
    {
        pthread_cond_broadcast(&codex->dongles[i].dongle_cond);
        i++;
    }
}
void *monitor_over_coders(void *arg)
{
    t_codexion *codex = (t_codexion *)arg;
    int lazy;
    while(!finished_simulation(codex))
    {
        lazy = detect_lazy_coder(codex->coders);
        if (lazy != -42)
        {
            modify_sim_status(codex, 1);
            wake_coders(codex);
            coder_logs(codex, get_time_ms(), lazy, "burned out");
            return NULL;
        }
        if (is_done(codex->coders))
        {
            modify_sim_status(codex, 1);
            wake_coders(codex);
            return NULL;
        }
    }
    return NULL;
}
