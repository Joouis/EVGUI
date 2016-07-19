/* #include "twin_calc.h" */
/* #include "twin_text.h" */
#include "twin_svg.h"
#include "twin_clock.h"
#include "system.h"

twin_fbdev_t *tf;

int main(void)
{

	tf = twin_fbdev_create();
	if ( tf == NULL ) {
		return 1;
	}

#if 0
	twin_text_start(tf->screen, "Text", 0, 0, 240, 320);
#endif
#if 1
    twin_clock_start (tf->screen, "Clock", 0, 0, 240, 320);
#endif
#if 1
	twin_svg_index_start (tf->screen, "screen", 0, 0, 240, 320);
#endif
#if 1
	twin_svg_music_start (tf->screen, "screen", 0, 0, 240, 320);
#endif

	twin_fbdev_activate(tf);

	twin_dispatch();

	return 0;
}
