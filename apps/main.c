#include "twin_calc.h"
#include "twin_text.h"
#include "system.h"

twin_fbdev_t *tf;

static unsigned int twin_def_cursor_image[] = {
        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,

        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,

        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,

        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x88ffffff, 0xff000000, 0xff000000, 0x88ffffff,
        0x00000000, 0x88ffffff, 0x88ffffff, 0x00000000,
};

static twin_pixmap_t *twin_get_default_cursor(int *hx, int *hy)
{
    twin_pixmap_t *cur;
    twin_pointer_t data;

    data.argb32 = twin_def_cursor_image;
    cur = twin_pixmap_create_const(TWIN_ARGB32, 4+4, 4+4, 16+16, data);
    if (cur == NULL)
        return NULL;
    *hx = *hy = 4;
    return cur;
}

int main(void)
{
	int hx, hy;
	twin_pixmap_t *cur;

	tf = twin_fbdev_create();
	if ( tf == NULL ) {
		return 1;
	}
	/* cur = twin_get_default_cursor(&hx, &hy); */
	/* if (cur != NULL) */
	/* 	twin_screen_set_cursor(tf->screen, cur, hx, hy); */
	/* twin_screen_set_background (tf->screen, twin_make_pattern ()); */

#if 0
	twin_calc_start(tf->screen, "Calculator", 0, 0, 60*3/2, 120*3/2);
#endif
/* #if 1 */
     twin_text_start (tf->screen,  "Gettysburg Address", 0, 0, 50, 50);
/* #endif */

	twin_fbdev_activate(tf);

	twin_dispatch();

	return 0;
}
