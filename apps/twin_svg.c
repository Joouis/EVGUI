#include <string.h>
#include "twin_svg.h"
#include "svgtiny.h"

#define D(x) twin_double_to_fixed(x)
#define TEXT_SIZE	9

twin_window_t  *index_page;

void render_path(twin_pixmap_t *dst, twin_path_t *svg_path, struct svgtiny_shape *path)
{
	
	unsigned int j;

	for ( j = 0; j != path->path_length; ) {
		switch ((int) path->path[j]) {
			case svgtiny_PATH_MOVE:
				twin_path_move (svg_path,
						D(path->path[j + 1]),
						D(path->path[j + 2]));
				j += 3;
				break;
			case svgtiny_PATH_CLOSE:
				twin_path_close (svg_path);
				j += 1;
				break;
			case svgtiny_PATH_LINE:
				twin_path_draw(svg_path,
						D(path->path[j + 1]),
						D(path->path[j + 2]));
				j += 3;
				break;
			case svgtiny_PATH_BEZIER:
				twin_path_curve(svg_path,
						D(path->path[j + 1]),
						D(path->path[j + 2]),
						D(path->path[j + 3]),
						D(path->path[j + 4]),
						D(path->path[j + 5]),
						D(path->path[j + 6]));
				j += 7;
				break;
			default:
				printf("error");
				j += 1;
		}
	}
	if (path->fill != svgtiny_TRANSPARENT) {
		twin_paint_path(dst, path->fill, svg_path);
	}
	if (path->stroke != svgtiny_TRANSPARENT) {
		twin_path_set_cap_style (svg_path, TwinCapProjecting);
		twin_paint_stroke (dst, path->stroke, svg_path, D(path->stroke_width));
	}
	twin_path_empty(svg_path);
}

void render_text(twin_pixmap_t *dst, twin_path_t *svg_path, struct svgtiny_shape *path)
{
	twin_path_move (svg_path,
			D(path->text_x),
			D(path->text_y));
	twin_path_utf8(svg_path, path->text);
	twin_paint_path(dst, path->stroke, svg_path);
	twin_path_empty(svg_path);
}

void
twin_svg_start (twin_screen_t *screen, const char *name, int x, int y, int w, int h)
{
    index_page = twin_window_create (screen, TWIN_ARGB32,
						TwinWindowApplication,
						x,y,w,h);
	unsigned int i;

    twin_fill (index_page->pixmap, 0xffffffff, TWIN_SOURCE,
	       0, 0,
	       index_page->client.right - index_page->client.left,
	       index_page->client.bottom - index_page->client.top);

	struct svgtiny_diagram *diagram = svgtiny_create();
	if (diagram) {
		svgtiny_code code = svgtiny_parse(diagram, test, strlen(test), "http://dontcare", 240, 320);
		if (svgtiny_OK == code) {

			twin_path_t *svg_path = twin_path_create();
			/* twin_path_set_font_size(svg_path, D(TEXT_SIZE)); */
			
			for (i = 0; i != diagram->shape_count; i++) {
				if (diagram->shape[i].path){
					render_path(index_page->pixmap, svg_path, &diagram->shape[i]);
				}else if (diagram->shape[i].text) {
					render_text(index_page->pixmap, svg_path, &diagram->shape[i]);
				}
			}

			twin_path_destroy(svg_path);
		} else {
			/* TODO: Show error message on the screen */
			
			twin_path_t *path = twin_path_create();
			twin_path_set_font_size(path, D(TEXT_SIZE));
			twin_fixed_t    fx, fy;
			static const char	*lines[] = {
				"Fourscore and seven years ago our fathers brought forth on",
				"this continent a new nation, conceived in liberty and",
				"dedicated to the proposition that all men are created equal.",
				0
			};
			const char **l;

			fx = D(3);
			fy = D(10);
			for (l = lines; *l; l++) 
			{
				twin_path_move (path, fx, fy);
				twin_path_utf8 (path, *l);
				twin_paint_path (index_page->pixmap, 0xff000000, path);
				twin_path_empty (path);
				fy += D(TEXT_SIZE);
			}
		}
		svgtiny_free(diagram);
	}

	twin_window_show(index_page);
}
