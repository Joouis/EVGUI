#include "twin_calc.h"
#include "system.h"

twin_fbdev_t *tf;

int main(void)
{
	tf = twin_fbdev_create();
	if ( tf == NULL ) {
		return 1;
	}

#if 1
	twin_calc_start(tf->screen, "Calculator", 0, 0, 60, 120);
#endif

	twin_fbdev_activate(tf);

	twin_dispatch();

	return 0;
}
