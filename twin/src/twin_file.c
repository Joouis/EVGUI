/*
 * Twin - A Tiny Window System
 * Copyright © 2004 Keith Packard <keithp@keithp.com>
 * All rights reserved.
 */

#include "twinint.h"

#include <assert.h>

static twin_queue_t	*head;

static twin_order_t
_twin_file_order (twin_queue_t *a, twin_queue_t *b)
{
    /* twin_file_t	*af = (twin_file_t *) a; */
    /* twin_file_t	*bf = (twin_file_t *) b; */
    
	// TODO: there is no fd for ordering
    /* if (af->file < bf->file) */
	/* return TWIN_BEFORE; */
    /* if (af->file > bf->file) */
	/* return TWIN_AFTER; */
    /* return TWIN_AT; */

	return TWIN_AFTER;
}

void
_twin_run_file (twin_time_t delay)
{
    twin_file_t	*first;
    twin_file_t	*file;
    
    first = (twin_file_t *) _twin_queue_set_order (&head);
    if (first)
    {
		for (file = first; file; file = (twin_file_t *) file->queue.order)
		{
			if (!(*file->proc) (file->closure))
				_twin_queue_delete (&head, &file->queue);
		}
		_twin_queue_review_order (&first->queue);
    }
    else
    {
	if (delay > 0)
	    /* usleep (delay * 1000); */
		while ( delay  ) delay--;
    }
}

twin_file_t *
twin_set_file (twin_file_proc_t	file_proc,
		  void			*closure)
{
    twin_file_t	*file = malloc (sizeof (twin_file_t));

    if (!file)
	return 0;

    file->proc = file_proc;
    file->closure = closure;
    
    _twin_queue_insert (&head, _twin_file_order, &file->queue);
    return file;
}

void
twin_clear_file (twin_file_t *file)
{
    _twin_queue_delete (&head, &file->queue);
}
