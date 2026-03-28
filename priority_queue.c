/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szyn <szyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 09:44:16 by szyn              #+#    #+#             */
/*   Updated: 2026/03/27 08:50:46 by szyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#define PARENT_POS(x) (((x) - 1) / 2)


void classic_swap(t_coder *linus, t_coder *torvalds)
{
    t_coder kernel = *linus;
    *linus = *torvalds;
    *torvalds = kernel;
}
void fix_upward(t_min_heap *heap, int coder_pos)
{    
    while (coder_pos > 0 && heap->coders[PARENT_POS(coder_pos)].priority > heap->coders[coder_pos].priority)
    {
        classic_swap(&heap->coders[PARENT_POS(coder_pos)], &heap->coders[coder_pos]);
        coder_pos = PARENT_POS(coder_pos);
    }
}
void fix_downward(t_min_heap *heap)
{
    int min;
    int w;
    int e;
    int coder_pos;
    
    coder_pos = 0;
    while (true)
    {
        w = 2 * coder_pos + 1;
        e = 2 * coder_pos + 2;
        min = coder_pos;
        if (w < heap->size && heap->coders[coder_pos].priority > heap->coders[w].priority)
            min = w;
        if (e < heap->size && heap->coders[min].priority > heap->coders[e].priority)
            min = e;
        if (min == coder_pos)
            break;
        classic_swap(&heap->coders[coder_pos], &heap->coders[min]);
        coder_pos = min;
    }
}

void pop_coder(t_min_heap *heap)
{
    t_coder coder = heap->coders[0];
    heap->coders[0] = heap->coders[heap->size-1];
    heap->size--;
    fix_downward(heap);
}
void push_coder(t_min_heap *heap, int id, int priority)
{
    heap->coders[heap->size].id = id;
    heap->coders[heap->size].priority = priority;
    fix_upward(heap, heap->size);
    heap->size++;
}

bool init_heap(t_min_heap *heap, int total_coders)
{
    heap->coders = malloc(sizeof(t_coder) * total_coders);
    if(!heap->coders)
        return false;
    heap->size = 0;
    return true;
}
