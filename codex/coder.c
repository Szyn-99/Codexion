#include "codexion.h"

/*
** Always acquire the lower-indexed dongle first.
** This breaks the circular wait that causes deadlock.
**
** Example with 4 coders:
**   coder 1: left=0  right=1  -> acquires 0 then 1
**   coder 2: left=1  right=2  -> acquires 1 then 2
**   coder 3: left=2  right=3  -> acquires 2 then 3
**   coder 4: left=3  right=0  -> acquires 0 then 3  <- reversed
**
** Without this, coder 4 holding dongle 3 and waiting for dongle 0
** while coder 1 holds dongle 0 and waits for dongle 1 while...
** -> circular wait -> deadlock.
*/
static void	acquire_both(t_coder *coder)
{
	t_sim	*sim;
	int		left;
	int		right;

	sim = coder->sim;
	left = coder->id - 1;
	right = coder->id % sim->nb_coders;
	if (left < right)
	{
		dongle_acquire(coder, &sim->dongles[left]);
		dongle_acquire(coder, &sim->dongles[right]);
	}
	else
	{
		dongle_acquire(coder, &sim->dongles[right]);
		dongle_acquire(coder, &sim->dongles[left]);
	}
}

static void	release_both(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	dongle_release(coder, &sim->dongles[coder->id - 1]);
	dongle_release(coder, &sim->dongles[coder->id % sim->nb_coders]);
}

static void	do_compile(t_coder *coder)
{
	coder->last_compile_start = get_time_ms();
	log_action(coder->sim, coder->id, "is compiling");
	usleep(coder->sim->time_to_compile * 1000);
	coder->compile_count++;
}

static void	*single_coder_wait(t_sim *sim)
{
	while (!sim->over)
		usleep(1000);
	return (NULL);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	if (sim->nb_coders == 1)
		return (single_coder_wait(sim));
	while (!sim->over)
	{
		acquire_both(coder);
		if (sim->over)
		{
			release_both(coder);
			break ;
		}
		do_compile(coder);
		release_both(coder);
		if (sim->over)
			break ;
		log_action(sim, coder->id, "is debugging");
		usleep(sim->time_to_debug * 1000);
		if (sim->over)
			break ;
		log_action(sim, coder->id, "is refactoring");
		usleep(sim->time_to_refactor * 1000);
	}
	return (NULL);
}
