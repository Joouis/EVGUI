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
	MUSIC = 2,
} SCREEN_TYPE;
SCREEN_TYPE screen_type = INDEX;

extern twin_window_t *index_page;
extern twin_window_t *music_page;

#ifdef CPU_LOADING
static char print_buffer[30];
#endif

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
					}else if ( (TP_State->Y > 215) && (TP_State->Y < 312) ) {
						twin_window_show(music_page);
						twin_window_hide(index_page);
						screen_type = MUSIC;
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
			case MUSIC:
				if ( (TP_State->X > 0) && (TP_State->X < 240 ) )
				{
					if ( (TP_State->Y > 288) && (TP_State->Y < 320 ) )
					{
						twin_window_show(index_page);
						twin_window_hide(music_page);
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
#ifdef CPU_LOADING
	UG_FontSelect(&FONT_12X20);
	UG_SetBackcolor(C_WHITE);
	UG_SetForecolor(C_RED);
#endif
    for (;;)
    {
#ifdef CPU_LOADING
	twin_time_t t1 = twin_now();
#endif

	_twin_run_timeout ();
	_twin_run_work ();
	_twin_touch_detection();

#ifdef CPU_LOADING
	twin_time_t t2 = twin_now();
	sprintf( print_buffer, "CPU Loading:%d", (t2-t1) );
	UG_PutString( 0, 0, print_buffer );
#endif
    }
}
