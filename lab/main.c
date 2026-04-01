#include "codexion.h"

static int	launch_threads(t_sim *sim, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
		{
			sim->over = 1;
			wake_all(sim);
			return (0);
		}
		i++;
	}
	if (pthread_create(monitor, NULL, monitor_routine, sim) != 0)
	{
		sim->over = 1;
		wake_all(sim);
		return (0);
	}
	return (1);
}

static void	join_threads(t_sim *sim, pthread_t monitor)
{
	int	i;

	pthread_join(monitor, NULL);
	i = 0;
	while (i < sim->nb_coders)
		pthread_join(sim->coders[i++].thread, NULL);
}

int	main(int argc, char **argv)
{
	t_sim		sim;
	pthread_t	monitor;

	if (!parse_args(argc, argv, &sim))
		return (1);
	if (!sim_init(&sim))
	{
		fprintf(stderr, "Error: failed to initialize simulation\n");
		return (1);
	}
	if (!launch_threads(&sim, &monitor))
	{
		join_threads(&sim, monitor);
		sim_free(&sim);
		return (1);
	}
	join_threads(&sim, monitor);
	sim_free(&sim);
	return (0);
}
