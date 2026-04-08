/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_free_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:34:01 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/09 00:11:42 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_dongles(t_codexion *codex)
{
	int	i;

	if (codex && codex->usbs)
	{
		i = 0;
		while (i < codex->parse->n_dongles)
		{
			pthread_mutex_destroy(&codex->usbs[i].usb_mutex);
			pthread_cond_destroy(&codex->usbs[i].usb_cond);
			i++;
		}
		free(codex->usbs);
	}
}

bool	destroy_resources(t_codexion *codex, int n_mutex, int n_cond)
{
	int	i;

	i = 0;
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
	free(codex->usbs);
	return (false);
}

bool	clean_abort(t_codexion *cdx, int level)
{
	if (level == 4)
	{
		pthread_mutex_destroy(&cdx->codex_mutex);
		pthread_mutex_destroy(&cdx->log_mutex);
	}
	if (level >= 3)
		free(cdx->coders);
	if (level >= 2)
		free_dongles(cdx);
	if (level >= 1)
		free(cdx->parse);
	free(cdx);
	return (false);
}
