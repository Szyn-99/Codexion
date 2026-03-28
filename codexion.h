/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:02:24 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/22 10:45:40 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <bool.h>

#define PARENT_POS(x) (((x) - 1) / 2)
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

// some complexe things
typedef struct s_coder
{
	int priority;
	int id;
} t_coder;

typedef struct s_min_heap
{
	t_coder *coders;
	int size;
	int total;
} t_min_heap;


// pthreads phase
void *compiling(void *a);
#endif
