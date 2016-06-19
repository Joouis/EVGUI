/*
 * Twin - A Tiny Window System
 * Copyright © 2004 Keith Packard <keithp@keithp.com>
 * All rights reserved.
 */

#include "twinint.h"

/* Touch structure */
static TP_STATE* TP_State;
typedef enum {
	INDEX = 0,
	CLOCK = 1,
} SCREEN_TYPE;
SCREEN_TYPE screen_type = INDEX;

extern twin_window_t *index_page;

void _twin_touch_detection ()
{
	TP_State = IOE_TP_GetState();
	if( TP_State->TouchDetected )
	{
		switch ( screen_type ) {
			case INDEX:
				if ( (TP_State->X > 10) && (TP_State->X < 117 ) )
				{
					if ( (TP_State->Y > 8) && (TP_State->Y < 105 ) )
					{
						twin_window_hide(index_page);
						screen_type = CLOCK;
					}
				}
				break;
			case CLOCK:
				if ( (TP_State->X > 80) && (TP_State->X < 160 ) )
				{
					if ( (TP_State->Y > 150) && (TP_State->Y < 200 ) )
					{
						twin_window_show(index_page);
						screen_type = INDEX;
					}
				}
				break;
			default:;
		}
	}
}

void
twin_dispatch (void)
{
    for (;;)
    {
	_twin_run_timeout ();
	_twin_run_work ();
	_twin_touch_detection();
    }
}
