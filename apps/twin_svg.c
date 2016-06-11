#include <string.h>
#include "twin_svg.h"
#include "svgtiny.h"

#define D(x) twin_double_to_fixed(x)
#define TEXT_SIZE	9

void render_path(twin_pixmap_t *dst, twin_path_t *svg_path, struct svgtiny_shape *path, float scale)
{
	
	// TODO: scale is not implement
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

void render_text(twin_pixmap_t *dst, twin_path_t *svg_path, struct svgtiny_shape *path, float scale)
{
	twin_path_move (svg_path,
			D(path->text_x),
			D(path->text_y));
	twin_path_utf8(svg_path, path->text);
	twin_paint_path(dst, path->stroke, svg_path);
	twin_path_empty(svg_path);
}

void
twin_svg_start (twin_screen_t *screen, const char *name, int x, int y, int w, int h, float scale)
{
    twin_window_t   *svg = twin_window_create (screen, TWIN_ARGB32,
						TwinWindowApplication,
						x,y,w,h);
	unsigned int i;

    /* twin_window_set_name(svg, name); */
    twin_fill (svg->pixmap, 0xffffffff, TWIN_SOURCE,
	       0, 0,
	       svg->client.right - svg->client.left,
	       svg->client.bottom - svg->client.top);

	struct svgtiny_diagram *diagram = svgtiny_create();
	if (diagram) {
		svgtiny_code code = svgtiny_parse(diagram, test, strlen(test), "http://dontcare", 1000, 1000);
		if (svgtiny_OK == code) {

			twin_path_t *svg_path = twin_path_create();
			/* twin_path_set_font_size(svg_path, D(TEXT_SIZE)); */
			
			for (i = 0; i != diagram->shape_count; i++) {
				if (diagram->shape[i].path){
					render_path(svg->pixmap, svg_path, &diagram->shape[i], scale);
				}else if (diagram->shape[i].text) {
					render_text(svg->pixmap, svg_path, &diagram->shape[i], scale);
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
				"",
				"Now we are engaged in a great civil war, testing whether that",
				"nation or any nation so conceived and so dedicated can long",
				"endure. We are met on a great battlefield of that war. We",
				"have come to dedicate a portion of it as a final resting",
				"place for those who died here that the nation might live.",
				"This we may, in all propriety do. But in a larger sense, we",
				"cannot dedicate, we cannot consecrate, we cannot hallow this",
				"ground. The brave men, living and dead who struggled here",
				"have hallowed it far above our poor power to add or detract.",
				"The world will little note nor long remember what we say here,",
				"but it can never forget what they did here.",
				"",
				"It is rather for us the living, we here be dedicated to the",
				"great task remaining before us--that from these honored",
				"dead we take increased devotion to that cause for which they",
				"here gave the last full measure of devotion--that we here",
				"highly resolve that these dead shall not have died in vain, that",
				"this nation shall have a new birth of freedom, and that",
				"government of the people, by the people, for the people shall",
				"not perish from the earth.",
				0
			};
			const char **l;

			fx = D(3);
			fy = D(10);
			for (l = lines; *l; l++) 
			{
				twin_path_move (path, fx, fy);
				twin_path_utf8 (path, *l);
				twin_paint_path (svg->pixmap, 0xff000000, path);
				twin_path_empty (path);
				fy += D(TEXT_SIZE);
			}
		}
		svgtiny_free(diagram);
	}

	twin_window_show(svg);
}
