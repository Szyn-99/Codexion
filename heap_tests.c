#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── structs ─────────────────────────────────────────────── */

typedef struct s_entry
{
	int		id;
	long	priority;
}	t_entry;

typedef struct s_heap
{
	t_entry	*data;
	int		size;
	int		capacity;
}	t_heap;

/* ─── internal helpers ────────────────────────────────────── */

static void	swap(t_entry *a, t_entry *b)
{
	t_entry	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *h, int i)
{
	while (i > 0 && h->data[(i - 1) / 2].priority > h->data[i].priority)
	{
		swap(&h->data[(i - 1) / 2], &h->data[i]);
		i = (i - 1) / 2;
	}
}

static void	sift_down(t_heap *h, int i)
{
	int	l;
	int	r;
	int	smallest;

	while (1)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		smallest = i;
		if (l < h->size && h->data[l].priority < h->data[smallest].priority)
			smallest = l;
		if (r < h->size && h->data[r].priority < h->data[smallest].priority)
			smallest = r;
		if (smallest == i)
			break ;
		swap(&h->data[i], &h->data[smallest]);
		i = smallest;
	}
}

/* ─── public API ──────────────────────────────────────────── */

int	heap_init(t_heap *h, int capacity)
{
	h->data = malloc(sizeof(t_entry) * capacity);
	if (!h->data)
		return (-1);
	h->size = 0;
	h->capacity = capacity;
	return (0);
}

void	heap_push(t_heap *h, int id, long priority)
{
	if (h->size >= h->capacity)
	{
		fprintf(stderr, "heap_push: heap is full (capacity=%d)\n", h->capacity);
		return ;
	}
	h->data[h->size].id = id;
	h->data[h->size].priority = priority;
	sift_up(h, h->size);
	h->size++;
}

t_entry	heap_pop(t_heap *h)
{
	t_entry	top;

	/* return a sentinel if empty */
	if (h->size == 0)
	{
		top.id = -1;
		top.priority = -1;
		return (top);
	}
	top = h->data[0];
	h->size--;
	h->data[0] = h->data[h->size];
	sift_down(h, 0);
	return (top);
}

void	heap_free(t_heap *h)
{
	free(h->data);
	h->data = NULL;
	h->size = 0;
	h->capacity = 0;
}

/* ─── test utilities ──────────────────────────────────────── */

static void	print_heap(t_heap *h, const char *label)
{
	int	i;

	printf("  %-30s  size=%-2d  [", label, h->size);
	i = 0;
	while (i < h->size)
	{
		printf("(id=%d p=%ld)", h->data[i].id, h->data[i].priority);
		if (i < h->size - 1)
			printf(", ");
		i++;
	}
	printf("]\n");
}

/* verify that every parent <= its children */
static int	heap_is_valid(t_heap *h)
{
	int	i;
	int	l;
	int	r;

	i = 0;
	while (i < h->size)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		if (l < h->size && h->data[l].priority < h->data[i].priority)
			return (0);
		if (r < h->size && h->data[r].priority < h->data[i].priority)
			return (0);
		i++;
	}
	return (1);
}

static void	assert_ok(int cond, const char *msg)
{
	if (cond)
		printf("  \033[32mPASS\033[0m  %s\n", msg);
	else
		printf("  \033[31mFAIL\033[0m  %s\n", msg);
}

/* ─── individual tests ────────────────────────────────────── */

static void	test_init(void)
{
	t_heap	h;

	printf("\n── test_init ───────────────────────────────\n");
	assert_ok(heap_init(&h, 8) == 0,     "heap_init returns 0 on success");
	assert_ok(h.size == 0,               "size is 0 after init");
	assert_ok(h.capacity == 8,           "capacity matches argument");
	assert_ok(h.data != NULL,            "data pointer is non-NULL");
	heap_free(&h);
	assert_ok(h.data == NULL,            "data pointer is NULL after free");
}

static void	test_push_single(void)
{
	t_heap	h;

	printf("\n── test_push_single ────────────────────────\n");
	heap_init(&h, 8);
	heap_push(&h, 1, 500);
	print_heap(&h, "after push (id=1, p=500)");
	assert_ok(h.size == 1,                      "size is 1");
	assert_ok(h.data[0].id == 1,               "root id is 1");
	assert_ok(h.data[0].priority == 500,        "root priority is 500");
	assert_ok(heap_is_valid(&h),               "heap property holds");
	heap_free(&h);
}

static void	test_push_ascending(void)
{
	t_heap	h;

	printf("\n── test_push_ascending (already sorted) ───\n");
	heap_init(&h, 8);
	heap_push(&h, 1, 100);
	heap_push(&h, 2, 200);
	heap_push(&h, 3, 300);
	heap_push(&h, 4, 400);
	print_heap(&h, "after 4 pushes (100,200,300,400)");
	assert_ok(h.data[0].priority == 100, "min (100) is at root");
	assert_ok(heap_is_valid(&h),         "heap property holds");
	heap_free(&h);
}

static void	test_push_descending(void)
{
	t_heap	h;

	printf("\n── test_push_descending (reverse order) ───\n");
	heap_init(&h, 8);
	heap_push(&h, 4, 400);
	heap_push(&h, 3, 300);
	heap_push(&h, 2, 200);
	heap_push(&h, 1, 100);
	print_heap(&h, "after 4 pushes (400,300,200,100)");
	assert_ok(h.data[0].priority == 100, "min (100) bubbled to root");
	assert_ok(heap_is_valid(&h),         "heap property holds");
	heap_free(&h);
}

static void	test_push_random(void)
{
	t_heap	h;

	printf("\n── test_push_random ────────────────────────\n");
	heap_init(&h, 8);
	heap_push(&h, 5, 800);
	heap_push(&h, 1, 200);
	heap_push(&h, 3, 500);
	heap_push(&h, 2, 100);
	heap_push(&h, 4, 350);
	print_heap(&h, "after 5 random pushes");
	assert_ok(h.data[0].priority == 100, "min (100) is at root");
	assert_ok(heap_is_valid(&h),         "heap property holds");
	heap_free(&h);
}

static void	test_pop_order(void)
{
	t_heap	h;
	t_entry	e;
	long	prev;

	printf("\n── test_pop_order (must come out ascending) \n");
	heap_init(&h, 8);
	heap_push(&h, 3, 500);
	heap_push(&h, 1, 100);
	heap_push(&h, 5, 800);
	heap_push(&h, 2, 200);
	heap_push(&h, 4, 350);
	prev = -1;
	while (h.size > 0)
	{
		e = heap_pop(&h);
		printf("  popped id=%d  priority=%ld\n", e.id, e.priority);
		assert_ok(e.priority >= prev,    "each pop >= previous pop");
		assert_ok(heap_is_valid(&h),     "heap property holds after pop");
		prev = e.priority;
	}
	heap_free(&h);
}

static void	test_pop_empty(void)
{
	t_heap	h;
	t_entry	e;

	printf("\n── test_pop_empty ──────────────────────────\n");
	heap_init(&h, 4);
	e = heap_pop(&h);
	assert_ok(e.id == -1,        "pop on empty heap returns sentinel id=-1");
	assert_ok(e.priority == -1,  "pop on empty heap returns sentinel priority=-1");
	heap_free(&h);
}

static void	test_push_full(void)
{
	t_heap	h;

	printf("\n── test_push_full (overflow guard) ─────────\n");
	heap_init(&h, 2);
	heap_push(&h, 1, 100);
	heap_push(&h, 2, 200);
	/* this push should print an error and NOT corrupt memory */
	heap_push(&h, 3, 50);
	assert_ok(h.size == 2,       "size stays at capacity (2) after overflow push");
	assert_ok(heap_is_valid(&h), "heap still valid after overflow attempt");
	heap_free(&h);
}

static void	test_duplicate_priorities(void)
{
	t_heap	h;
	t_entry	e;
	long	prev;

	printf("\n── test_duplicate_priorities ───────────────\n");
	heap_init(&h, 8);
	heap_push(&h, 1, 300);
	heap_push(&h, 2, 300);
	heap_push(&h, 3, 100);
	heap_push(&h, 4, 300);
	heap_push(&h, 5, 100);
	print_heap(&h, "5 entries, priorities: 300,300,100,300,100");
	assert_ok(h.data[0].priority == 100, "min (100) is at root");
	assert_ok(heap_is_valid(&h),         "heap property holds");
	prev = -1;
	while (h.size > 0)
	{
		e = heap_pop(&h);
		assert_ok(e.priority >= prev, "pops are non-decreasing with duplicates");
		prev = e.priority;
	}
	heap_free(&h);
}

/* simulate FIFO scheduler: priority = arrival timestamp */
static void	test_fifo_simulation(void)
{
	t_heap	h;
	t_entry	e;

	printf("\n── test_fifo_simulation ────────────────────\n");
	printf("  3 coders request dongle at different times:\n");
	heap_init(&h, 8);
	heap_push(&h, 2, 400);  /* coder 2 arrived at t=400ms */
	heap_push(&h, 3, 100);  /* coder 3 arrived at t=100ms */
	heap_push(&h, 1, 250);  /* coder 1 arrived at t=250ms */
	print_heap(&h, "waiters (sorted by arrival)");
	e = heap_pop(&h);
	printf("  dongle granted to coder %d (arrived at t=%ldms)\n", e.id, e.priority);
	assert_ok(e.id == 3,          "coder 3 gets dongle (arrived first at t=100)");
	e = heap_pop(&h);
	printf("  dongle granted to coder %d (arrived at t=%ldms)\n", e.id, e.priority);
	assert_ok(e.id == 1,          "coder 1 gets dongle next (t=250)");
	e = heap_pop(&h);
	printf("  dongle granted to coder %d (arrived at t=%ldms)\n", e.id, e.priority);
	assert_ok(e.id == 2,          "coder 2 gets dongle last (t=400)");
	heap_free(&h);
}

/* simulate EDF scheduler: priority = last_compile + time_to_burnout */
static void	test_edf_simulation(void)
{
	t_heap	h;
	t_entry	e;

	printf("\n── test_edf_simulation ─────────────────────\n");
	printf("  3 coders with different burnout deadlines:\n");
	heap_init(&h, 8);
	heap_push(&h, 1, 1400); /* coder 1 burns out at t=1400ms */
	heap_push(&h, 2, 2000); /* coder 2 burns out at t=2000ms */
	heap_push(&h, 3, 950);  /* coder 3 burns out at t=950ms  */
	print_heap(&h, "waiters (sorted by deadline)");
	e = heap_pop(&h);
	printf("  dongle granted to coder %d (deadline t=%ldms)\n", e.id, e.priority);
	assert_ok(e.id == 3,     "coder 3 gets dongle (closest to burnout at t=950)");
	e = heap_pop(&h);
	printf("  dongle granted to coder %d (deadline t=%ldms)\n", e.id, e.priority);
	assert_ok(e.id == 1,     "coder 1 gets dongle next (deadline t=1400)");
	e = heap_pop(&h);
	printf("  dongle granted to coder %d (deadline t=%ldms)\n", e.id, e.priority);
	assert_ok(e.id == 2,     "coder 2 gets dongle last (deadline t=2000)");
	heap_free(&h);
}

static void	test_push_pop_interleaved(void)
{
	t_heap	h;
	t_entry	e;

	printf("\n── test_push_pop_interleaved ───────────────\n");
	heap_init(&h, 8);
	heap_push(&h, 1, 300);
	heap_push(&h, 2, 100);
	e = heap_pop(&h);
	printf("  popped id=%d p=%ld\n", e.id, e.priority);
	assert_ok(e.priority == 100,  "first pop gives 100");
	heap_push(&h, 3, 50);
	heap_push(&h, 4, 200);
	print_heap(&h, "after interleaved pushes/pops");
	assert_ok(h.data[0].priority == 50,  "new min (50) is at root");
	assert_ok(heap_is_valid(&h),          "heap valid after interleaved ops");
	e = heap_pop(&h);
	assert_ok(e.priority == 50,   "pop gives 50");
	e = heap_pop(&h);
	assert_ok(e.priority == 200,  "pop gives 200");
	e = heap_pop(&h);
	assert_ok(e.priority == 300,  "pop gives 300");
	assert_ok(h.size == 0,        "heap is empty");
	heap_free(&h);
}

/* ─── main ────────────────────────────────────────────────── */

int	main(void)
{
	printf("╔══════════════════════════════════════════╗\n");
	printf("║           heap test suite                ║\n");
	printf("╚══════════════════════════════════════════╝\n");

	test_init();
	test_push_single();
	test_push_ascending();
	test_push_descending();
	test_push_random();
	test_pop_order();
	test_pop_empty();
	test_push_full();
	test_duplicate_priorities();
	test_fifo_simulation();
	test_edf_simulation();
	test_push_pop_interleaved();

	printf("\n────────────────────────────────────────────\n");
	printf("All tests done.\n");
	return (0);
}