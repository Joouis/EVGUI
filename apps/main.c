#include "twin_calc.h"
#include "twin_text.h"
#include "twin_svg.h"
#include "system.h"

twin_fbdev_t *tf;

int main(void)
{

	tf = twin_fbdev_create();
	if ( tf == NULL ) {
		return 1;
	}
	/* twin_screen_set_background (tf->screen, twin_make_pattern ()); */

#if 0
	twin_calc_start(tf->screen, "Calculator", 0, 0, 60*3/2, 120*3/2);
#endif
#if 0
    twin_text_start (tf->screen,  "Gettysburg Address", 0, 0, 240, 320);
#endif
#if 1
    twin_clock_start (tf->screen, "Clock", 0, 0, 240, 320);
#endif
#if 0
	twin_svg_start (tf->screen, "screen", 0, 0, 240, 320);
#endif

	twin_fbdev_activate(tf);

	twin_dispatch();

	return 0;
}
