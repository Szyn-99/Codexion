/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_free_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:34:01 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/30 11:52:53 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void free_coders(t_codexion *codex)
{
    int i = 0;
    while (i < codex->parse->number_of_coders)
    {
        free(&codex->coders[i]);
        i++;
    }
}

bool destroy_resources(t_codexion *codex, int n_mutex, int n_cond)
{
    int i = 0;
    while (i <= n_mutex)
    {
        pthread_mutex_destroy(&codex->dongles[i].dongle_mutex);
        i++;
    }
    i = 0;
    while (i <= n_cond)
    {
        pthread_cond_destroy(&codex->dongles[i].dongle_cond);
        i++;
    }
    return false;
}