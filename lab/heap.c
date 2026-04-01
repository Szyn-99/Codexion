#include "codexion.h"

static void	swap(t_waiter *a, t_waiter *b)
{
	t_waiter	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *heap, int i)
{
	while (i > 0
		&& heap->data[PARENT(i)].priority > heap->data[i].priority)
	{
		swap(&heap->data[PARENT(i)], &heap->data[i]);
		i = PARENT(i);
	}
}

static void	sift_down(t_heap *heap)
{
	int	i;
	int	l;
	int	r;
	int	min;

	i = 0;
	while (1)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		min = i;
		if (l < heap->size
			&& heap->data[l].priority < heap->data[min].priority)
			min = l;
		if (r < heap->size
			&& heap->data[r].priority < heap->data[min].priority)
			min = r;
		if (min == i)
			break ;
		swap(&heap->data[i], &heap->data[min]);
		i = min;
	}
}

bool	heap_init(t_heap *heap, int nb_coders)
{
	heap->data = malloc(sizeof(t_waiter) * nb_coders);
	if (!heap->data)
		return (false);
	heap->size = 0;
	return (true);
}

void	heap_push(t_heap *heap, int id, long priority)
{
	heap->data[heap->size].id = id;
	heap->data[heap->size].priority = priority;
	sift_up(heap, heap->size);
	heap->size++;
}

t_waiter	heap_pop(t_heap *heap)
{
	t_waiter	top;

	top = heap->data[0];
	heap->size--;
	heap->data[0] = heap->data[heap->size];
	sift_down(heap);
	return (top);
}

void	heap_free(t_heap *heap)
{
	free(heap->data);
	heap->data = NULL;
	heap->size = 0;
}
