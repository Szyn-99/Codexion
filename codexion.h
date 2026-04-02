/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 04:02:24 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/02 10:41:55 by szyn             ###   ########.fr       */
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

void free_parser(t_codexion *codex);
struct s_coder
{
	pthread_t		thread;
	t_codexion		*sim;
	long			last_compile_start;
	int				id;
	int				compiles;
};
void free_dongles(t_codexion *codex);
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


/* ========== parsing_arguments.c ========== */
void		ft_codexion_parser(t_parse *main, char **av, int ac);
int			yes_space(char c);
int			ft_atoi(char *str);
int			yes_int(char c);
void		init_coders_metadata(t_parse *main, char **av);
int			parse_scheduler(char *s, t_parse *main);

/* ========== codexion.c ========== */
int			exit_error(char *message);

/* ========== codexion_free_utils.c ========== */
void		free_coders(t_codexion *codex);
bool		destroy_resources(t_codexion *codex, int n_mutex, int n_cond);

/* ========== codexion_init.c ========== */
bool		coders_init(t_codexion *cdx);
bool		dongles_init(t_codexion *cdx);
bool		codexion_init(t_codexion **cdx, char **av, int ac);
bool		parser_init(t_codexion *cdx, char **av, int ac);
void		wake_coders(t_codexion *codex);
/* ========== codexion_utils.c ========== */
long		get_time_ms(void);
void		coder_logs(t_codexion *codex, long timestamp, int id, char *log);
long		choose_priority(t_coder *coder);
int		finished_simulation(t_codexion *codex);
void modify_sim_status(t_codexion *codex, int status);
void free_all(t_codexion *codex);
/* ========== priority_queue.c ========== */
void		classic_swap(t_waiter *linus, t_waiter *torvalds);
void		push_coder(int id, long priority, t_heap *heap);
void		remove_coder(t_heap *heap);
/* ========== codexion_croutine.c ========== */
void		take_dongle(t_codexion *codex, t_coder *coder, int dongle_pos);
void		take_two_dongles(t_codexion *codex, t_coder *coder, int rd, int ld);
void		put_dongle(t_codexion *codex, int dongle_pos);
void		coders_phases(t_coder *coder, int phase);
void		*coders_routine(void *arg);

/* ========== codexion_monitor.c ========== */
int			detect_lazy_coder(t_codexion *codex);
bool		is_done(t_codexion *codex);
void		*monitor_over_coders(void *arg);

/* ========== codexion_release_catch.c ========== */
int			release_coders(t_codexion *codex);
bool		release_monitor(t_codexion *codex);
void		catch_coders(t_codexion *codex, int succeeded);
#endif
