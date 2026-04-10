/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 09:44:16 by aymel-ha          #+#    #+#             */
/*   Updated: 2026/04/10 10:27:38 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	classic_swap(t_waiter *linus, t_waiter *torvalds)
{
	t_waiter	kernel;

	kernel = *linus;
	*linus = *torvalds;
	*torvalds = kernel;
}

void	push_coder(int id, long priority, t_heap *heap)
{
	heap->waiters[heap->size].id = id;
	heap->waiters[heap->size].priority = priority;
	heap->size++;
	if (heap->size == 2
		&& heap->waiters[0].priority > heap->waiters[1].priority)
		classic_swap(&heap->waiters[0], &heap->waiters[1]);
}

void	remove_coder(t_heap *heap)
{
	if (heap->size == 2)
		heap->waiters[0] = heap->waiters[1];
	heap->size--;
}

int	yes_space(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	yes_int(char c)
{
	return (c >= '0' && c <= '9');
}
