/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:02:24 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/03/30 12:23:13 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
#include <sys/time.h>


#define PARENT_POS(x) (((x) - 1) / 2)

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

typedef struct s_waiter
{
	int		id;
	long	priority;
}	t_waiter;

typedef struct s_heap
{
	t_waiter	waiters[2];
	int			size;
}	t_heap;

typedef struct s_dongle
{
	t_heap			queue;
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	dongle_cond;
	long			available_at;
	int				in_use;
}	t_dongle;

typedef struct s_coder t_coder;

typedef struct s_codexion t_codexion;

struct s_coder
{
	pthread_t		thread;
	t_codexion		*sim;
	long			last_compile_start;
	int				id;
	int				compiles;
};

struct s_codexion
{
	t_dongle		*dongles;
	t_coder			*coders;
	t_parse			*parse;
	pthread_t		monitor;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	sim_mutex;
	int				sim_over;
	long			start;
}	;


void		ft_codexion_parser(t_parse *main, char **av, int ac);
int			yes_space(char c);
int			ft_atoi(char *str);
int			yes_int(char c);

// free utils
void free_coders(t_codexion *codex);
bool destroy_resources(t_codexion *codex, int n_mutex, int n_cond);
// pthreads phase
#endif
