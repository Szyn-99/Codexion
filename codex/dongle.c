#include "codexion.h"

bool	dongle_init(t_dongle *d, int nb_coders)
{
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
		return (false);
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&d->mutex);
		return (false);
	}
	d->in_use = 0;
	d->cooldown_until = 0;
	if (!heap_init(&d->heap, nb_coders))
	{
		pthread_mutex_destroy(&d->mutex);
		pthread_cond_destroy(&d->cond);
		return (false);
	}
	return (true);
}

static long	get_priority(t_coder *coder)
{
	if (strcmp(coder->sim->scheduler, "edf") == 0)
		return (coder->last_compile_start + coder->sim->time_to_burnout);
	return (get_time_ms());
}

static int	my_turn(t_coder *coder, t_dongle *d)
{
	return (d->heap.data[0].id == coder->id
		&& !d->in_use
		&& get_time_ms() >= d->cooldown_until);
}

/*
** Wait with a deadline when we are at the top of the heap but cooldown
** has not expired yet. This avoids the missed-wakeup bug where the coder
** registers in the heap after the dongle is already free, then sleeps
** forever because nobody broadcasts again for the cooldown expiry.
**
** pthread_cond_timedwait wakes up automatically at cooldown_until so the
** coder can re-check and take the dongle without waiting for a broadcast.
*/
static void	wait_for_turn(t_coder *coder, t_dongle *d)
{
	struct timespec	ts;
	struct timeval	tb;
	long			deadline_ms;
	long			remaining;

	while (!coder->sim->over && !my_turn(coder, d))
	{
		if (d->heap.data[0].id == coder->id && !d->in_use)
		{
			/* we are next — just waiting for cooldown to expire */
			deadline_ms = d->cooldown_until;
			remaining = deadline_ms - get_time_ms();
			if (remaining <= 0)
				break ;
			ts.tv_sec = deadline_ms / 1000;
			ts.tv_nsec = (deadline_ms % 1000) * 1000000L;
			pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
		}
		else
			pthread_cond_wait(&d->cond, &d->mutex);
	}
}

void	dongle_acquire(t_coder *coder, t_dongle *d)
{
	long	priority;

	pthread_mutex_lock(&d->mutex);
	if (!d->in_use
		&& get_time_ms() >= d->cooldown_until
		&& d->heap.size == 0)
	{
		d->in_use = 1;
		pthread_mutex_unlock(&d->mutex);
		log_action(coder->sim, coder->id, "has taken a dongle");
		return ;
	}
	priority = get_priority(coder);
	heap_push(&d->heap, coder->id, priority);
	wait_for_turn(coder, d);
	if (!coder->sim->over)
	{
		heap_pop(&d->heap);
		d->in_use = 1;
		pthread_mutex_unlock(&d->mutex);
		log_action(coder->sim, coder->id, "has taken a dongle");
		return ;
	}
	heap_pop(&d->heap);
	pthread_mutex_unlock(&d->mutex);
}

void	dongle_release(t_coder *coder, t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->in_use = 0;
	d->cooldown_until = get_time_ms() + coder->sim->cooldown_ms;
	pthread_mutex_unlock(&d->mutex);
	pthread_cond_broadcast(&d->cond);
}

void	dongle_free(t_dongle *d)
{
	heap_free(&d->heap);
	pthread_cond_destroy(&d->cond);
	pthread_mutex_destroy(&d->mutex);
}
