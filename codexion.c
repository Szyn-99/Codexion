/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:08:08 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/22 08:29:11 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_parse	*data;

	if (ac != 9)
	{
		printf("Error: Invalid Number of Arguments\n");
		return (1);
	}
	data = malloc(sizeof(t_parse));
	ft_codexion_parser(data, av, ac);
	if (data->parse_result)
	{
		free(data);
		return (1);
	}
	pthread_t coders;
	pthread_create(&coders, NULL, compiling, NULL);
	pthread_join(coders, NULL);
	printf("number_of_coders: %d\n", data->number_of_coders);
	printf("time_to_burnout: %d\n", data->time_to_burnout);
	printf("time_to_compile: %d\n", data->time_to_compile);
	printf("time_to_debug: %d\n", data->time_to_debug);
	printf("time_to_refactor: %d\n", data->time_to_refactor);
	printf("number_of_compiles_required: %d\n",
		data->number_of_compiles_required);
	printf("dongle_cooldown: %d\n", data->dongle_cooldown);
	printf("scheduler: %s\n", data->scheduler);
	printf("parse_result: %d\n", data->parse_result);
	free(data);
}
// ./codexion 20 1 2 89 3 7 5  edf