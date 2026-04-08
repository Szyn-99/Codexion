/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:06:56 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/02 10:43:49 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_atoi(char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (yes_space(str[i]))
		i++;
	if (!str[i])
		return (-42);
	while (str[i] != '\0')
	{
		if (yes_int(str[i]))
		{
			if (((result * 10) + (str[i] - '0')) > 2147483647)
				return (-42);
			result = (result * 10) + (str[i] - '0');
		}
		else if (!yes_space(str[i]))
			return (-42);
		i++;
	}
	return (result);
}

void	init_coders_metadata(t_parse *main, char **av)
{
	main->n_coders = ft_atoi(av[1]);
	main->n_dongles = main->n_coders;
	main->t_burnout = ft_atoi(av[2]);
	main->t_compile = ft_atoi(av[3]);
	main->t_debug = ft_atoi(av[4]);
	main->t_refactor = ft_atoi(av[5]);
	main->n_compiles = ft_atoi(av[6]);
	main->dt_cooldown = ft_atoi(av[7]);
	main->parse_result = 0;
}

int	parse_scheduler(char *s, t_parse *main)
{
	if (s[0] == 'f' && s[1] == 'i' && s[2] == 'f' && s[3] == 'o'
		&& s[4] == '\0')
		main->scheduler = "fifo";
	else if (s[0] == 'e' && s[1] == 'd' && s[2] == 'f' && s[3] == '\0')
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
	int	j;

	j = 1;
	if (parse_scheduler(av[ac - 1], main) == -42)
	{
		return ;
	}
	while (j < ac - 1)
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
