/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:08:08 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/01 19:33:28 by aymel-ha         ###   ########.fr       */
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

	if (ac != 9)
		return exit_error("Error: Invalid Number of Arguments\n");
	if (!codexion_init(&codexion, av, ac))
		return exit_error("Error: Failed to initialize the simulation\n");
	int succeded_release = release_coders(codexion);
	release_monitor(codexion);
	catch_coders(codexion, succeded_release);
	
		
}
	
