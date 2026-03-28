#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

# define PARENT(i) (((i) - 1) / 2)

typedef struct s_waiter
{
	int		id;
	long	priority;
}	t_waiter;

typedef struct s_heap
{
	t_waiter	*data;
	int			size;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				in_use;
	long			cooldown_until;
	t_heap			heap;
}	t_dongle;

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	long		last_compile_start;
	int			compile_count;
	t_sim		*sim;
}	t_coder;

struct s_sim
{
	t_coder			*coders;
	t_dongle		*dongles;
	int				nb_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				nb_compiles_req;
	long			cooldown_ms;
	char			*scheduler;
	long			sim_start;
	volatile int	over;
	pthread_mutex_t	log_mutex;
};

bool		heap_init(t_heap *heap, int nb_coders);
void		heap_push(t_heap *heap, int id, long priority);
t_waiter	heap_pop(t_heap *heap);
void		heap_free(t_heap *heap);

bool	dongle_init(t_dongle *d, int nb_coders);
void	dongle_acquire(t_coder *coder, t_dongle *d);
void	dongle_release(t_coder *coder, t_dongle *d);
void	dongle_free(t_dongle *d);

void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);

long	get_time_ms(void);
void	log_action(t_sim *sim, int id, const char *msg);
void	wake_all(t_sim *sim);
int		parse_args(int argc, char **argv, t_sim *sim);
int		sim_init(t_sim *sim);
void	sim_free(t_sim *sim);

#endif
