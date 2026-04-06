/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 10:50:16 by szyn              #+#    #+#             */
/*   Updated: 2026/04/02 10:38:56 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool coders_init(t_codexion *cdx)
{
    cdx->coders = malloc(sizeof(t_coder) * cdx->parse->number_of_coders);
    if (!cdx->coders)
        return false;
    int i = 0;
    int	ld;
    int	rd;
    while (i < cdx->parse->number_of_coders)
    {
        cdx->coders[i].id = i + 1;
        cdx->coders[i].last_compile_start = 0;
        cdx->coders[i].compiles = 0;
        cdx->coders[i].sim = cdx;
        ld = i;
        rd = (i + 1) % cdx->parse->number_of_coders;
        if (ld < rd)
        {
            cdx->coders[i].d1 = &cdx->dongles[ld];
            cdx->coders[i].d2 = &cdx->dongles[rd];
        }
        else
        {
            cdx->coders[i].d1 = &cdx->dongles[rd];
            cdx->coders[i].d2 = &cdx->dongles[ld];
        }
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
            return destroy_resources(cdx, i, i);
        if (pthread_cond_init(&cdx->dongles[i].dongle_cond, NULL))
            return destroy_resources(cdx, i+1, i);
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
    free_coders(cdx);
    free_dongles(cdx);
    free_parser(cdx);
}
bool	codexion_init(t_codexion **cdx, char **av, int ac)
{
	*cdx = malloc(sizeof(t_codexion));
	if (!*cdx)
		return (false);
	if (!parser_init(*cdx, av, ac))
		return (free(*cdx), false);
	if (!dongles_init(*cdx))
	{
		free_parser(*cdx);
		free(*cdx);
		return (false);
	}
	if (!coders_init(*cdx))
	{
		free_dongles(*cdx);
		free_parser(*cdx);
		free(*cdx);
		return (false);
	}
	if (pthread_mutex_init(&(*cdx)->sim_mutex, NULL))
	{
		free_all(*cdx);
		free(*cdx);
		return (false);
	}
	if (pthread_mutex_init(&(*cdx)->log_mutex, NULL))
	{
		pthread_mutex_destroy(&(*cdx)->sim_mutex);
		free_all(*cdx);
		free(*cdx);
		return (false);
	}
	(*cdx)->sim_over = 0;
	return (true);
}
