/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 10:50:16 by szyn              #+#    #+#             */
/*   Updated: 2026/03/30 12:23:39 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

bool coders_init(t_codexion *cdx)
{
    cdx->start = get_time_ms();
    cdx->coders = malloc(sizeof(t_coder) * cdx->parse->number_of_coders);
    if (cdx->coders)
        return false;
    int i = 0;
    while (i < cdx->parse->number_of_coders)
    {
        cdx->coders[i].id = i + 1;
        cdx->coders[i].last_compile_start = cdx->start;
        cdx->coders[i].compiles = 0;
        cdx->coders[i].sim = cdx;
        i++;     
    }
    return true;
}
bool dongles_init(t_codexion *cdx)
{
    cdx->dongles = malloc(sizeof(t_dongle) * cdx->parse->number_of_dongles);
    if (!cdx->dongles)
        return false;
    int i = 0;
    while (i < cdx->parse->number_of_dongles)
    {
        if(pthread_mutex_init(&cdx->dongles[i].dongle_mutex, NULL))
            return destroy_resources(cdx, i, i - 1);
        if (pthread_cond_init(&cdx->dongles[i].dongle_cond, NULL))
            return destroy_resources(cdx, i, i);
        cdx->dongles[i].available_at = 0;
        cdx->dongles[i].queue.size = 0;
        cdx->dongles[i].in_use = 0;
        i++;     
    }
    return true;
}
bool codexion_init(t_codexion *cdx)
{
    if (!coders_init(cdx))
        return false;
    if (!dongles_init(cdx))
        {
            free_coders(cdx);
            return false;
        }
    return true;
}