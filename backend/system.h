// ------------------------------------
// µGUI example project
// http://www.embeddedlightning.com/
// ------------------------------------


#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "ugui.h"
#include "ltdc.h"
#include "sdram.h"
#include "ili9341.h"
#include "delay.h"
#include "stm32f429i_discovery_ioe.h"
#include "stm32f429i_discovery.h"
#include "twin.h"
#include "twinint.h"

#define WIDTH	240
#define HEIGHT	320

extern u32 ltdc_work_layer;

typedef struct _twin_fbdev{
	twin_screen_t *screen;	/* twin screen */
	uint16_t xres;
	uint16_t yres;

	int last_btn; /* last button state */

	char *fb_base;
	size_t fb_len;
	char *fb_ptr;
} twin_fbdev_t;

struct timeval{
	long tv_sec;
	long tv_msec;
};

struct tm {
    int tm_sec;    /* Seconds (0-60) */
    int tm_min;    /* Minutes (0-59) */
    int tm_hour;   /* Hours (0-23) */
    int tm_mday;   /* Day of the month (1-31) */
    int tm_mon;    /* Month (0-11) */
    int tm_year;   /* Year - 1900 */
    int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
    int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
    int tm_isdst;  /* Daylight saving time */
};

twin_fbdev_t *twin_fbdev_create(void);
twin_bool_t twin_fbdev_activate(twin_fbdev_t *tf);

#endif
