#include "codexion.h"

static int	find_burnout(t_sim *sim)
{
	int		i;
	long	now;

	now = get_time_ms();
	i = 0;
	while (i < sim->nb_coders)
	{
		if (now >= sim->coders[i].last_compile_start + sim->time_to_burnout)
			return (sim->coders[i].id);
		i++;
	}
	return (-1);
}

static int	all_done(t_sim *sim)
{
	int	i;

	if (sim->nb_compiles_req == 0)
		return (0);
	i = 0;
	while (i < sim->nb_coders)
	{
		if (sim->coders[i].compile_count < sim->nb_compiles_req)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		id;

	sim = (t_sim *)arg;
	while (!sim->over)
	{
		usleep(500);
		id = find_burnout(sim);
		if (id != -1)
		{
			sim->over = 1;
			wake_all(sim);
			log_action(sim, id, "burned out");
			return (NULL);
		}
		if (all_done(sim))
		{
			sim->over = 1;
			wake_all(sim);
			return (NULL);
		}
	}
	return (NULL);
}
