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
    cdx->coders = malloc(sizeof(t_coder) * cdx->parse->n_coders);
    if (!cdx->coders)
        return false;
    int i = 0;
    int	ld;
    int	rd;
    while (i < cdx->parse->n_coders)
    {
        cdx->coders[i].id = i + 1;
        cdx->coders[i].last_compile_start = 0;
        cdx->coders[i].compiles_done = 0;
        cdx->coders[i].codexion = cdx;
        ld = i;
        rd = (i + 1) % cdx->parse->n_coders;
        if (ld < rd)
        {
            cdx->coders[i].lowest_usb = &cdx->usbs[ld];
            cdx->coders[i].highest_usb = &cdx->usbs[rd];
        }
        else
        {
            cdx->coders[i].lowest_usb = &cdx->usbs[rd];
            cdx->coders[i].highest_usb = &cdx->usbs[ld];
        }
        i++;     
    }
    return true;
}
bool dongles_init(t_codexion *cdx)
{
    cdx->usbs = malloc(sizeof(t_usb) * cdx->parse->n_dongles);
    if (!cdx->usbs)
        return false;
    int i = 0;
    while (i < cdx->parse->n_dongles)
    {
        if(pthread_mutex_init(&cdx->usbs[i].usb_mutex, NULL))
            return destroy_resources(cdx, i, i);
        if (pthread_cond_init(&cdx->usbs[i].usb_cond, NULL))
            return destroy_resources(cdx, i+1, i);
        cdx->usbs[i].served_at = 0;
        cdx->usbs[i].queue.size = 0;
        cdx->usbs[i].free = 0;
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
	if (pthread_mutex_init(&(*cdx)->codex_mutex, NULL))
	{
		free_all(*cdx);
		free(*cdx);
		return (false);
	}
	if (pthread_mutex_init(&(*cdx)->log_mutex, NULL))
	{
		pthread_mutex_destroy(&(*cdx)->codex_mutex);
		free_all(*cdx);
		free(*cdx);
		return (false);
	}
	(*cdx)->codex_over = 0;
	return (true);
}
