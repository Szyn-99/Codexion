/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:02:24 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/09 07:28:23 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
// parsing phase
typedef struct s_parse
{
	int		number_of_coders;
	int		number_of_dongles;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	int		parse_result;
	char	*scheduler;
}			t_parse;
void		ft_codexion_parser(t_parse *main, char **av, int ac);
int			yes_space(char c);
int			ft_atoi(char *str);
int			yes_int(char c);

#endif
