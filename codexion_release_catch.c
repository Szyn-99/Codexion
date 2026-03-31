/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_release_catch.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 12:30:19 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/31 15:18:09 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int release_coders(t_codexion *codex)
{
    int i = 0;
    int check;
    while (i < codex->parse->number_of_coders)
    {
        check = pthread_create(&codex->coders[i].thread, NULL, coders_routine, codex->coders);
        if (check)
        {
            modify_sim_status(codex, 1);
            wake_coders(codex);
            return i;
        }
        i++;
    }
    return i;
}
bool release_monitor(t_codexion *codex)
{
    if(pthread_create(&codex->monitor, NULL, monitor_over_coders, codex))
    {
        modify_sim_status(codex, 1);
        wake_coders(codex);
        return false;
    }
    return true;
}
void catch_coders(t_codexion *codex, int succeeded)
{
    int i = 0;
    while(i < succeeded)
    {
        pthread_join(codex->coders[i].thread, NULL);
        i++;
    }
}
