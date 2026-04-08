/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_free_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:34:01 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/31 15:39:29 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void free_coders(t_codexion *codex)
{
    if (codex && codex->coders)
        free(codex->coders);
}
void free_dongles(t_codexion *codex)
{
    if (codex && codex->usbs)
    {
        int i = 0;
        while(i < codex->parse->n_dongles)
        {
            pthread_mutex_destroy(&codex->usbs[i].usb_mutex);
            pthread_cond_destroy(&codex->usbs[i].usb_cond);
            i++;
        }
        free(codex->usbs);
    }
}

void free_parser(t_codexion *codex)
{
    if (codex && codex->parse)
        free(codex->parse);
}

bool destroy_resources(t_codexion *codex, int n_mutex, int n_cond)
{
    int i = 0;
    while (i < n_mutex)
    {
        pthread_mutex_destroy(&codex->usbs[i].usb_mutex);
        i++;
    }
    i = 0;
    while (i < n_cond)
    {
        pthread_cond_destroy(&codex->usbs[i].usb_cond);
        i++;
    }
    return false;
}