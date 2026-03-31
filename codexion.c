/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:08:08 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/31 15:31:06 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	main(int ac, char **av)
{
	t_codexion	*codexion;

	if (ac != 9)
	{
		printf("Error: Invalid Number of Arguments\nGot: %d\n", ac-1);
		return (1);
	}
	codexion = malloc(sizeof(t_codexion));
	if (!codexion)
	{
		perror("malloc");
		return (1);
	}
	bool a = codexion_init(&codexion, av, ac);
	printf("%d\n", codexion->parse->number_of_coders);
	printf("Result: %s\n", a ? "true" : "false");
	free_all(codexion);
}
