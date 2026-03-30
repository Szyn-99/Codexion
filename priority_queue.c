/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aymel-ha <aymel-ha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 09:44:16 by szyn              #+#    #+#             */
/*   Updated: 2026/03/30 15:43:55 by aymel-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void classic_swap(t_waiter *linus, t_waiter *torvalds)
{
    t_waiter kernel = *linus;
    *linus = *torvalds;
    *torvalds = kernel;
}

void push_coder(int id, long priority, t_heap *heap)
{
    heap->waiters[heap->size].id = id;
    heap->waiters[heap->size].priority = priority;
    heap->size++;
    if (heap->size == 2 && heap->waiters[0].priority > heap->waiters[1].priority)
        classic_swap(&heap->waiters[0], &heap->waiters[1]);
}

void remove_coder(t_heap *heap)
{
    heap->waiters[0] = heap->waiters[1];
    heap->size--; 
}