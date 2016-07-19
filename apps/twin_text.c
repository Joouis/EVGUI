/*
 * Twin - A Tiny Window System
 * Copyright © 2004 Keith Packard <keithp@keithp.com>
 * All rights reserved.
 */

#include <twin_text.h>

#define D(x) twin_double_to_fixed(x)

void
twin_text_start (twin_screen_t *screen, const char *name, int x, int y, int w, int h)
{
    twin_window_t   *text = twin_window_create (screen, TWIN_ARGB32,
						TwinWindowApplication,
						x,y,w,h);
    twin_fixed_t    fx, fy;
    static const char	*lines[] = {
	"  ",
	"This is a test!",
	0
    };
    const char **l;
    twin_path_t	*path;
    
    twin_window_set_name(text, name);
    path = twin_path_create ();
#define TEXT_SIZE   30
    twin_path_set_font_size (path, D(TEXT_SIZE));
    fx = D(3);
    fy = D(10);
    twin_fill (text->pixmap, 0xc0c0c0c0, TWIN_SOURCE,
	       0, 0, 
	       text->client.right - text->client.left,
	       text->client.bottom - text->client.top);
    for (l = lines; *l; l++) 
    {
	twin_path_move (path, fx, fy);
	twin_path_utf8 (path, *l);
	twin_paint_path (text->pixmap, 0xff000000, path);
	twin_path_empty (path);
	fy += D(TEXT_SIZE);
    }
    twin_window_show (text);
}

