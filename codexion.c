/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:08:08 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/02 10:25:09 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int exit_error(char *message)
{
	printf("%s\n", message);
	return 1;
}

int	main(int ac, char **av)
{
	t_codexion	*codexion;
	int			succeeded;

	if (ac != 9)
		return (exit_error("Error: Invalid Number of Arguments\n"));
	if (!codexion_init(&codexion, av, ac))
		return (exit_error("Error: Failed to initialize the simulation\n"));
	succeeded = release_coders(codexion);
	if (!release_monitor(codexion))
	{
		free_all(codexion);
		free(codexion);
		return (exit_error("Error: Failed to launch monitor\n"));
	}
	catch_coders(codexion, succeeded);
	pthread_mutex_destroy(&codexion->sim_mutex);
	pthread_mutex_destroy(&codexion->log_mutex);
	free_all(codexion);
	free(codexion);
	return (0);
}
	
