/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:06:56 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/09 07:13:29 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_coders_metadata(t_parse *main, char **av)
{
	main->number_of_coders = ft_atoi(av[1]);
	main->number_of_dongles = main->number_of_coders;
	main->time_to_burnout = ft_atoi(av[2]);
	main->time_to_compile = ft_atoi(av[3]);
	main->time_to_debug = ft_atoi(av[4]);
	main->time_to_refactor = ft_atoi(av[5]);
	main->number_of_compiles_required = ft_atoi(av[6]);
	main->dongle_cooldown = ft_atoi(av[7]);
	main->parse_result = 0;
}

int	parse_scheduler(char *s, t_parse *main)
{
	if (s[0] == 'f' && s[1] == 'i' && s[2] == 'f' && s[3] == 'o')
		main->scheduler = "fifo";
	else if (s[0] == 'e' && s[1] == 'd' && s[2] == 'f')
		main->scheduler = "edf";
	else
	{
		printf("Error: Invalid Scheduler\n");
		main->parse_result = 1;
		return (-42);
	}
	return (1);
}

void	ft_codexion_parser(t_parse *main, char **av, int ac)
{
	int		j;
	char	*s;

	j = 1;
	s = av[ac - 1];
	if (parse_scheduler(av[ac - 1], main) == -42)
	{
		return ;
	}
	while (j < ac - 2)
	{
		if (ft_atoi(av[j]) == -42)
		{
			printf("Error: Invalid Input\n");
			main->parse_result = 1;
			return ;
		}
		j++;
	}
	init_coders_metadata(main, av);
}
