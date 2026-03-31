/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 10:50:16 by szyn              #+#    #+#             */
/*   Updated: 2026/03/31 15:31:02 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool coders_init(t_codexion *cdx)
{
    cdx->start = get_time_ms();
    cdx->coders = malloc(sizeof(t_coder) * cdx->parse->number_of_coders);
    if (!cdx->coders)
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

bool parser_init(t_codexion *cdx, char **av, int ac)
{
    cdx->parse = malloc(sizeof(t_parse));
    if(!cdx->parse)
        return false;
    ft_codexion_parser(cdx->parse, av, ac);
	if (cdx->parse->parse_result)
	{
		free(cdx->parse);
		return (false);
	}
    return true;
}
void free_all(t_codexion *cdx)
{
    free_parser(cdx);
    free_coders(cdx);
    free_dongles(cdx);
}
bool codexion_init(t_codexion **cdx, char **av, int ac)
{
    if(!parser_init(*cdx, av, ac))
        return false;
    if (!coders_init(*cdx))
    {
        free_parser(*cdx);
        return false;
    }
    if (!dongles_init(*cdx))
    {
        free_parser(*cdx);
        free_coders(*cdx);
        return false;
    }
    
    return true;
}