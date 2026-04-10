/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 10:10:22 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/10 10:10:23 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_parse
{
	int						n_coders;
	int						n_dongles;
	int						t_burnout;
	int						t_compile;
	int						t_debug;
	int						t_refactor;
	int						n_compiles;
	int						dt_cooldown;
	int						parse_result;
	char					*scheduler;
}							t_parse;

typedef struct s_waiter
{
	int						id;
	long					priority;
}							t_waiter;

typedef struct s_heap
{
	t_waiter				waiters[2];
	int						size;
}							t_heap;

typedef struct s_usb
{
	t_heap					queue;
	pthread_mutex_t			usb_mutex;
	pthread_cond_t			usb_cond;
	long					served_at;
	int						free;
}							t_usb;

typedef struct s_coder		t_coder;
typedef struct s_codexion	t_codexion;

struct						s_coder
{
	pthread_t				thread;
	t_codexion				*codexion;
	t_usb					*lowest_usb;
	t_usb					*highest_usb;
	long					last_compile_start;
	int						id;
	int						compiles_done;
};

struct						s_codexion
{
	t_usb					*usbs;
	t_coder					*coders;
	t_parse					*parse;
	pthread_t				monitor;
	pthread_mutex_t			log_mutex;
	pthread_mutex_t			codex_mutex;
	int						codex_over;
	long					start_time;
};

void						ft_codexion_parser(t_parse *main, char **av,
								int ac);
int							yes_space(char c);
int							ft_atoi(char *str);
int							yes_int(char c);
void						init_coders_metadata(t_parse *main, char **av);
int							parse_scheduler(char *s, t_parse *main);

int							exit_error(char *message);

void						free_dongles(t_codexion *codex);
bool						destroy_resources(t_codexion *codex, int n_mutex,
								int n_cond);

bool						coders_init(t_codexion *cdx);
bool						dongles_init(t_codexion *cdx);
bool						codexion_init(t_codexion **cdx, char **av, int ac);
bool						parser_init(t_codexion *cdx, char **av, int ac);
void						hand_dongles(t_codexion *cdx, int ld, int rd,
								int i);

long						current_time_ms(void);
void						coder_logs(t_codexion *codex, long timestamp,
								int id, char *log);
long						choose_priority(t_coder *coder);
int							ritual_ended(t_codexion *codex);
void						modify_ritual_status(t_codexion *codex, int status);
bool						clean_abort(t_codexion *cdx, int level);

void						classic_swap(t_waiter *linus, t_waiter *torvalds);
void						push_coder(int id, long priority, t_heap *heap);
void						remove_coder(t_heap *heap);

bool						usb_ready(t_usb *d, int id);
void						wait_for_usb(t_usb *d, int id);
void						bring_two_usbs(t_usb *lowest, t_usb *highest,
								int id, t_codexion *cdx);

bool						take_two_dongles(t_codexion *codex, t_coder *coder);
void						put_dongle(t_usb *usb, t_codexion *codex);
void						coders_phases(t_coder *coder, int phase);
bool						ritual(t_coder *coder);
void						*run_coders_ritual(void *arg);

int							detect_lazy_coder(t_codexion *codex);
bool						goal_achieved(t_codexion *codex);
void						wake_coders(t_codexion *codex);
void						*monitor_over_coders(void *arg);

int							release_coders(t_codexion *codex);
bool						release_monitor(t_codexion *codex);
void						catch_coders(t_codexion *codex, int succeeded);

#endif
