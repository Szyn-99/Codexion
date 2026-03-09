/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:08:08 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/09 07:17:08 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_parse	*coders_metada;

	if (ac != 9)
	{
		printf("Error: Invalid Number of Arguments\n");
		return (1);
	}
	coders_metada = malloc(sizeof(t_parse));
	ft_codexion_parser(coders_metada, av, ac);
	if (coders_metada->parse_result)
	{
		free(coders_metada);
		return (1);
	}
	printf("number_of_coders: %d\n", coders_metada->number_of_coders);
	printf("time_to_burnout: %d\n", coders_metada->time_to_burnout);
	printf("time_to_compile: %d\n", coders_metada->time_to_compile);
	printf("time_to_debug: %d\n", coders_metada->time_to_debug);
	printf("time_to_refactor: %d\n", coders_metada->time_to_refactor);
	printf("number_of_compiles_required: %d\n",
		coders_metada->number_of_compiles_required);
	printf("dongle_cooldown: %d\n", coders_metada->dongle_cooldown);
	printf("scheduler: %s\n", coders_metada->scheduler);
	printf("parse_result: %d\n", coders_metada->parse_result);
	free(coders_metada);
    FILE* file = 
}
