/*
 * Twin - A Tiny Window System
 * Copyright © 2004 Keith Packard <keithp@keithp.com>
 * All rights reserved.
 */

#include "twinint.h"

void
twin_dispatch (void)
{
    for (;;)
    {
	_twin_run_timeout ();
	_twin_run_work ();
	_twin_run_file (_twin_timeout_delay ());
    }
}
