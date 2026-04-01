#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

void	log_action(t_sim *sim, int id, const char *msg)
{
	long	ts;

	ts = get_time_ms() - sim->sim_start;
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d %s\n", ts, id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	wake_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_cond_broadcast(&sim->dongles[i].cond);
		i++;
	}
}

static int	is_digits(const char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	int	i;

	if (argc != 9)
	{
		fprintf(stderr, "Usage: %s nb_coders time_to_burnout time_to_compile"
			" time_to_debug time_to_refactor nb_compiles_req"
			" dongle_cooldown scheduler\n", argv[0]);
		return (0);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_digits(argv[i]))
		{
			fprintf(stderr, "Error: argument %d must be"
				" a non-negative integer\n", i);
			return (0);
		}
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n");
		return (0);
	}
	sim->nb_coders = atoi(argv[1]);
	if (sim->nb_coders < 1)
	{
		fprintf(stderr, "Error: need at least 1 coder\n");
		return (0);
	}
	sim->time_to_burnout = atol(argv[2]);
	sim->time_to_compile = atol(argv[3]);
	sim->time_to_debug = atol(argv[4]);
	sim->time_to_refactor = atol(argv[5]);
	sim->nb_compiles_req = atoi(argv[6]);
	sim->cooldown_ms = atol(argv[7]);
	sim->scheduler = argv[8];
	return (1);
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (!dongle_init(&sim->dongles[i], sim->nb_coders))
		{
			while (i > 0)
				dongle_free(&sim->dongles[--i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int	sim_init(t_sim *sim)
{
	int	i;

	sim->over = 0;
	sim->sim_start = get_time_ms();
	sim->coders = malloc(sizeof(t_coder) * sim->nb_coders);
	if (!sim->coders)
		return (0);
	sim->dongles = malloc(sizeof(t_dongle) * sim->nb_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		return (0);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	if (!init_dongles(sim))
	{
		free(sim->coders);
		free(sim->dongles);
		pthread_mutex_destroy(&sim->log_mutex);
		return (0);
	}
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].last_compile_start = sim->sim_start;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
		i++;
	}
	return (1);
}

void	sim_free(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
		dongle_free(&sim->dongles[i++]);
	free(sim->dongles);
	free(sim->coders);
	pthread_mutex_destroy(&sim->log_mutex);
}
