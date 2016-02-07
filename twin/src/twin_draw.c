/*
 * Twin - A Tiny Window System
 * Copyright © 2004 Keith Packard <keithp@keithp.com>
 * All rights reserved.
 */

#include "twinint.h"

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 1
#endif

/* op, src, dst */
static twin_src_op  comp2[2][4][3] = {
    {	/* OVER */
	{   /* A8 */
	    _twin_a8_over_a8,
	    _twin_a8_over_rgb16,
	    _twin_a8_over_argb32,
	},
	{   /* RGB16 */
	    _twin_rgb16_over_a8,
	    _twin_rgb16_over_rgb16,
	    _twin_rgb16_over_argb32,
	},
	{   /* ARGB32 */
	    _twin_argb32_over_a8,
	    _twin_argb32_over_rgb16,
	    _twin_argb32_over_argb32,
	},
	{   /* C */
	    _twin_c_over_a8,
	    _twin_c_over_rgb16,
	    _twin_c_over_argb32,
	}
    },
    {	/* SOURCE */
	{   /* A8 */
	    _twin_a8_source_a8,
	    _twin_a8_source_rgb16,
	    _twin_a8_source_argb32,
	},
	{   /* RGB16 */
	    _twin_rgb16_source_a8,
	    _twin_rgb16_source_rgb16,
	    _twin_rgb16_source_argb32,
	},
	{   /* ARGB32 */
	    _twin_argb32_source_a8,
	    _twin_argb32_source_rgb16,
	    _twin_argb32_source_argb32,
	},
	{   /* C */
	    _twin_c_source_a8,
	    _twin_c_source_rgb16,
	    _twin_c_source_argb32,
	}
    }
};

/* op, src, msk, dst */
static twin_src_msk_op	comp3[2][4][4][3] = {
    {	/* OVER */
	{   /* A8 */
	    {	/* A8 */
		_twin_a8_in_a8_over_a8,
		_twin_a8_in_a8_over_rgb16,
		_twin_a8_in_a8_over_argb32,
	    },
	    {	/* RGB16 */
		_twin_a8_in_rgb16_over_a8,
		_twin_a8_in_rgb16_over_rgb16,
		_twin_a8_in_rgb16_over_argb32,
	    },
	    {	/* ARGB32 */
		_twin_a8_in_argb32_over_a8,
		_twin_a8_in_argb32_over_rgb16,
		_twin_a8_in_argb32_over_argb32,
	    },
	    {	/* C */
		_twin_a8_in_c_over_a8,
		_twin_a8_in_c_over_rgb16,
		_twin_a8_in_c_over_argb32,
	    },
	},
	{   /* RGB16 */
	    {	/* A8 */
		_twin_rgb16_in_a8_over_a8,
		_twin_rgb16_in_a8_over_rgb16,
		_twin_rgb16_in_a8_over_argb32,
	    },
	    {	/* RGB16 */
		_twin_rgb16_in_rgb16_over_a8,
		_twin_rgb16_in_rgb16_over_rgb16,
		_twin_rgb16_in_rgb16_over_argb32,
	    },
	    {	/* ARGB32 */
		_twin_rgb16_in_argb32_over_a8,
		_twin_rgb16_in_argb32_over_rgb16,
		_twin_rgb16_in_argb32_over_argb32,
	    },
	    {	/* C */
		_twin_rgb16_in_c_over_a8,
		_twin_rgb16_in_c_over_rgb16,
		_twin_rgb16_in_c_over_argb32,
	    },
	},
	{   /* ARGB32 */
	    {	/* A8 */
		_twin_argb32_in_a8_over_a8,
		_twin_argb32_in_a8_over_rgb16,
		_twin_argb32_in_a8_over_argb32,
	    },
	    {	/* RGB16 */
		_twin_argb32_in_rgb16_over_a8,
		_twin_argb32_in_rgb16_over_rgb16,
		_twin_argb32_in_rgb16_over_argb32,
	    },
	    {	/* ARGB32 */
		_twin_argb32_in_argb32_over_a8,
		_twin_argb32_in_argb32_over_rgb16,
		_twin_argb32_in_argb32_over_argb32,
	    },
	    {	/* C */
		_twin_argb32_in_c_over_a8,
		_twin_argb32_in_c_over_rgb16,
		_twin_argb32_in_c_over_argb32,
	    },
	},
	{   /* C */
	    {	/* A8 */
		_twin_c_in_a8_over_a8,
		_twin_c_in_a8_over_rgb16,
		_twin_c_in_a8_over_argb32,
	    },
	    {	/* RGB16 */
		_twin_c_in_rgb16_over_a8,
		_twin_c_in_rgb16_over_rgb16,
		_twin_c_in_rgb16_over_argb32,
	    },
	    {	/* ARGB32 */
		_twin_c_in_argb32_over_a8,
		_twin_c_in_argb32_over_rgb16,
		_twin_c_in_argb32_over_argb32,
	    },
	    {	/* C */
		_twin_c_in_c_over_a8,
		_twin_c_in_c_over_rgb16,
		_twin_c_in_c_over_argb32,
	    },
	},
    },
    {	/* SOURCE */
	{   /* A8 */
	    {	/* A8 */
		_twin_a8_in_a8_source_a8,
		_twin_a8_in_a8_source_rgb16,
		_twin_a8_in_a8_source_argb32,
	    },
	    {	/* RGB16 */
		_twin_a8_in_rgb16_source_a8,
		_twin_a8_in_rgb16_source_rgb16,
		_twin_a8_in_rgb16_source_argb32,
	    },
	    {	/* ARGB32 */
		_twin_a8_in_argb32_source_a8,
		_twin_a8_in_argb32_source_rgb16,
		_twin_a8_in_argb32_source_argb32,
	    },
	    {	/* C */
		_twin_a8_in_c_source_a8,
		_twin_a8_in_c_source_rgb16,
		_twin_a8_in_c_source_argb32,
	    },
	},
	{   /* RGB16 */
	    {	/* A8 */
		_twin_rgb16_in_a8_source_a8,
		_twin_rgb16_in_a8_source_rgb16,
		_twin_rgb16_in_a8_source_argb32,
	    },
	    {	/* RGB16 */
		_twin_rgb16_in_rgb16_source_a8,
		_twin_rgb16_in_rgb16_source_rgb16,
		_twin_rgb16_in_rgb16_source_argb32,
	    },
	    {	/* ARGB32 */
		_twin_rgb16_in_argb32_source_a8,
		_twin_rgb16_in_argb32_source_rgb16,
		_twin_rgb16_in_argb32_source_argb32,
	    },
	    {	/* C */
		_twin_rgb16_in_c_source_a8,
		_twin_rgb16_in_c_source_rgb16,
		_twin_rgb16_in_c_source_argb32,
	    },
	},
	{   /* ARGB32 */
	    {	/* A8 */
		_twin_argb32_in_a8_source_a8,
		_twin_argb32_in_a8_source_rgb16,
		_twin_argb32_in_a8_source_argb32,
	    },
	    {	/* RGB16 */
		_twin_argb32_in_rgb16_source_a8,
		_twin_argb32_in_rgb16_source_rgb16,
		_twin_argb32_in_rgb16_source_argb32,
	    },
	    {	/* ARGB32 */
		_twin_argb32_in_argb32_source_a8,
		_twin_argb32_in_argb32_source_rgb16,
		_twin_argb32_in_argb32_source_argb32,
	    },
	    {	/* C */
		_twin_argb32_in_c_source_a8,
		_twin_argb32_in_c_source_rgb16,
		_twin_argb32_in_c_source_argb32,
	    },
	},
	{   /* C */
	    {	/* A8 */
		_twin_c_in_a8_source_a8,
		_twin_c_in_a8_source_rgb16,
		_twin_c_in_a8_source_argb32,
	    },
	    {	/* RGB16 */
		_twin_c_in_rgb16_source_a8,
		_twin_c_in_rgb16_source_rgb16,
		_twin_c_in_rgb16_source_argb32,
	    },
	    {	/* ARGB32 */
		_twin_c_in_argb32_source_a8,
		_twin_c_in_argb32_source_rgb16,
		_twin_c_in_argb32_source_argb32,
	    },
	    {	/* C */
		_twin_c_in_c_source_a8,
		_twin_c_in_c_source_rgb16,
		_twin_c_in_c_source_argb32,
	    },
	},
    }
};

    
#define operand_index(o)    ((o)->source_kind == TWIN_SOLID ? 3 : o->u.pixmap->format)

/* XXX Fixme: source clipping is busted
 */
static void _twin_composite_simple (twin_pixmap_t	*dst,
				    twin_coord_t	dst_x,
				    twin_coord_t	dst_y,
				    twin_operand_t	*src,
				    twin_coord_t	src_x,
				    twin_coord_t	src_y,
				    twin_operand_t	*msk,
				    twin_coord_t	msk_x,
				    twin_coord_t	msk_y,
				    twin_operator_t	operator,
				    twin_coord_t	width,
				    twin_coord_t	height)
{
    twin_coord_t    iy;
    twin_coord_t    left, top, right, bottom;
    twin_coord_t    sdx, sdy;
    twin_source_u   s;

    dst_x += dst->origin_x;
    dst_y += dst->origin_y;
    left = dst_x;
    top = dst_y;
    right = dst_x + width;
    bottom = dst_y + height;

    /* clip */
    if (left < dst->clip.left)
	left = dst->clip.left;
    if (top < dst->clip.top)
	top = dst->clip.top;
    if (right > dst->clip.right)
	right = dst->clip.right;
    if (bottom > dst->clip.bottom)
	bottom = dst->clip.bottom;

    if (left >= right || top >= bottom)
	return;

    if (src->source_kind == TWIN_PIXMAP) {
	src_x += src->u.pixmap->origin_x;
	src_y += src->u.pixmap->origin_y;
    } else
        s.c = src->u.argb;
    
    sdx = src_x - dst_x;
    sdy = src_y - dst_y;

    if (msk) {
	twin_src_msk_op	op;
	twin_source_u   m;
	twin_coord_t	mdx, mdy;
	
	if (msk->source_kind == TWIN_PIXMAP) {
	    msk_x += msk->u.pixmap->origin_x;
	    msk_y += msk->u.pixmap->origin_y;
	} else
	    m.c = msk->u.argb;
	
	mdx = msk_x - dst_x;
	mdy = msk_y - dst_y;

	op = comp3[operator][operand_index(src)][operand_index(msk)][dst->format];
	for (iy = top; iy < bottom; iy++)
	{
	    if (src->source_kind == TWIN_PIXMAP)
		s.p = twin_pixmap_pointer (src->u.pixmap, left+sdx, iy+sdy);
	    if (msk->source_kind == TWIN_PIXMAP)
		m.p = twin_pixmap_pointer (msk->u.pixmap, left+mdx, iy+mdy);
	    (*op) (twin_pixmap_pointer (dst, left, iy), s, m, right - left);
	}
    }
    else
    {
	twin_src_op	op;
	
	op = comp2[operator][operand_index(src)][dst->format];
	
	for (iy = top; iy < bottom; iy++)
	{
	    if (src->source_kind == TWIN_PIXMAP)
		s.p = twin_pixmap_pointer (src->u.pixmap, left+sdx, iy+sdy);
	    (*op) (twin_pixmap_pointer (dst, left, iy), s, right - left);
	}
    }
    twin_pixmap_damage (dst, left, top, right, bottom);
}
	     
static inline int operand_xindex(twin_operand_t	*o)
{
    int ind = operand_index(o);

    return ind != TWIN_RGB16 ? ind : TWIN_ARGB32;
}

static twin_xform_t *twin_pixmap_init_xform (twin_pixmap_t *pixmap,
					     twin_coord_t left, twin_coord_t width,
					     twin_coord_t src_x, twin_coord_t src_y)
{
    twin_xform_t	*xform;
    twin_format_t	fmt = pixmap->format;

    if (fmt == TWIN_RGB16)
	fmt = TWIN_ARGB32;

    xform = calloc(1, sizeof (twin_xform_t) +
		   width * twin_bytes_per_pixel(fmt));
    if (xform == NULL)
	return NULL;

    xform->span.v = (twin_argb32_t *)(char *)(xform + 1);
    xform->pixmap = pixmap;
    xform->left = left;
    xform->width = width;
    xform->src_x = src_x;
    xform->src_y = src_y;

    return xform;
}

static void twin_pixmap_free_xform (twin_xform_t *xform)
{
    if (xform)
	free (xform);
}

#define FX(x)	twin_int_to_fixed(x)
#define XF(x)	twin_fixed_to_int(x)

/* we are doing clipping on source... dunno if that makes much sense
 * but here we go ... if we decide that source clipping makes no sense
 * then we need to still test wether we fit in the pixmap boundaries
 * here. source clipping is useful if you try to extract one image
 * out of a big picture though.
 */
#define _pix_clipped(pix, x, y) \
	((x) < FX((pix)->clip.left) || (x) >= FX((pix)->clip.right) || \
	 (y) < FX((pix)->clip.top)  || (y) >= FX((pix)->clip.bottom))

#define _get_pix_8(d, pix, x, y) \
    do { \
	(d) = _pix_clipped(pix, x, y) ? 0 : \
	          *((pix)->p.a8 + XF(y) * (pix)->stride + XF(x)); \
    } while(0)

#define _get_pix_16(d, pix, x, y) \
    do { \
	twin_rgb16_t p = _pix_clipped(pix, x, y) ? 0 : \
	    *((pix)->p.argb32 + XF(y) * ((pix)->stride >> 1) + XF(x)); \
	*((twin_argb32_t *)(char *)(d)) = twin_rgb16_to_argb32(p); \
    } while(0)

#define _get_pix_32(d, pix, x, y) \
    do { \
	twin_argb32_t p = _pix_clipped(pix, x, y) ? 0 : \
	    *((pix)->p.argb32 + XF(y) * ((pix)->stride >> 2) + XF(x)); \
	*((twin_argb32_t *)(char *)(d)) = p; \
    } while(0)


#define _pix_saucemix(tl, tr, bl, br, wx, wy) \
	(( ((((br * wx) + (bl * (TWIN_FIXED_ONE - wx))) >> 16) \
		* wy) + \
	   ((((tr * wx) + (tl * (TWIN_FIXED_ONE - wx))) >> 16) \
		* (TWIN_FIXED_ONE - wy)) \
	 ) >> 16)


static void twin_pixmap_read_xform_8 (twin_xform_t *xform, twin_coord_t line)
{
    twin_fixed_t dx, dy, sx, sy;
    unsigned int wx, wy;
    twin_a8_t pts[4];
    twin_a8_t *dst = xform->span.a8;
    twin_pixmap_t *pix = xform->pixmap;
    twin_matrix_t *tfm = &xform->pixmap->transform;

    /* for each row in the dest line */
    dy = twin_int_to_fixed(line);
    for (dx = 0; dx < twin_int_to_fixed(xform->width); dx += TWIN_FIXED_ONE) {
	sx = _twin_matrix_fx(tfm, dx, dy) + FX(xform->src_x);
	sy = _twin_matrix_fy(tfm, dx, dy) + FX(xform->src_y);
	_get_pix_8(pts[0], pix, sx, sy);
	_get_pix_8(pts[1], pix, sx + TWIN_FIXED_ONE, sy);
	_get_pix_8(pts[2], pix, sx, sy + TWIN_FIXED_ONE);
	_get_pix_8(pts[3], pix, sx, sy + TWIN_FIXED_ONE);
	wx = sx & 0xffff;
	wy = sy & 0xffff;
	*(dst++) = _pix_saucemix(pts[0], pts[1], pts[2], pts[4], wx, wy);
    }
}

static void twin_pixmap_read_xform_16 (twin_xform_t *xform, twin_coord_t line)
{
    twin_fixed_t dx, dy, sx, sy;
    unsigned int wx, wy;
    twin_a8_t pts[4][4];
    twin_a8_t *dst = xform->span.a8;
    twin_pixmap_t *pix = xform->pixmap;
    twin_matrix_t *tfm = &xform->pixmap->transform;

    /* for each row in the dest line */
    dy = twin_int_to_fixed(line);
    for (dx = 0; dx < twin_int_to_fixed(xform->width); dx += TWIN_FIXED_ONE) {
	sx = _twin_matrix_fx(tfm, dx, dy) + FX(xform->src_x);
	sy = _twin_matrix_fy(tfm, dx, dy) + FX(xform->src_y);
	_get_pix_16(pts[0], pix, sx, sy);
	_get_pix_16(pts[1], pix, sx + TWIN_FIXED_ONE, sy);
	_get_pix_16(pts[2], pix, sx, sy + TWIN_FIXED_ONE);
	_get_pix_16(pts[3], pix, sx + TWIN_FIXED_ONE, sy + TWIN_FIXED_ONE);
	wx = sx & 0xffff;
	wy = sy & 0xffff;
	*(dst++) = _pix_saucemix(pts[0][0], pts[1][0],
				 pts[2][0], pts[3][0], wx, wy);
	*(dst++) = _pix_saucemix(pts[0][1], pts[1][1],
				 pts[2][1], pts[3][1], wx, wy);
	*(dst++) = _pix_saucemix(pts[0][2], pts[1][2],
				 pts[2][2], pts[3][2], wx, wy);
	*(dst++) = _pix_saucemix(pts[0][3], pts[1][3],
				 pts[2][3], pts[3][3], wx, wy);
    }
}

static void twin_pixmap_read_xform_32 (twin_xform_t *xform, twin_coord_t line)
{
    twin_fixed_t dx, dy, sx, sy;
    unsigned int wx, wy;
    twin_a8_t pts[4][4];
    twin_a8_t *dst = xform->span.a8;
    twin_pixmap_t *pix = xform->pixmap;
    twin_matrix_t *tfm = &xform->pixmap->transform;

    /* for each row in the dest line */
    dy = twin_int_to_fixed(line);
    for (dx = 0; dx < twin_int_to_fixed(xform->width); dx += TWIN_FIXED_ONE) {
	sx = _twin_matrix_fx(tfm, dx, dy) + FX(xform->src_x);
	sy = _twin_matrix_fy(tfm, dx, dy) + FX(xform->src_y);
	_get_pix_32(pts[0], pix, sx, sy);
	_get_pix_32(pts[1], pix, sx + TWIN_FIXED_ONE, sy);
	_get_pix_32(pts[2], pix, sx, sy + TWIN_FIXED_ONE);
	_get_pix_32(pts[3], pix, sx + TWIN_FIXED_ONE, sy + TWIN_FIXED_ONE);
	wx = sx & 0xffff;
	wy = sy & 0xffff;
	*(dst++) = _pix_saucemix(pts[0][0], pts[1][0],
				 pts[2][0], pts[3][0], wx, wy);
	*(dst++) = _pix_saucemix(pts[0][1], pts[1][1],
				 pts[2][1], pts[3][1], wx, wy);
	*(dst++) = _pix_saucemix(pts[0][2], pts[1][2],
				 pts[2][2], pts[3][2], wx, wy);
	*(dst++) = _pix_saucemix(pts[0][3], pts[1][3],
				 pts[2][3], pts[3][3], wx, wy);
    }
}

static void twin_pixmap_read_xform (twin_xform_t *xform, twin_coord_t line)
{
    if (xform->pixmap->format == TWIN_A8)
	twin_pixmap_read_xform_8(xform, line);
    else if (xform->pixmap->format == TWIN_RGB16)
	twin_pixmap_read_xform_16(xform, line);
    else if (xform->pixmap->format == TWIN_ARGB32)
	twin_pixmap_read_xform_32(xform, line);
}

static void _twin_composite_xform (twin_pixmap_t	*dst,
				   twin_coord_t		dst_x,
				   twin_coord_t		dst_y,
				   twin_operand_t	*src,
				   twin_coord_t		src_x,
				   twin_coord_t		src_y,
				   twin_operand_t	*msk,
				   twin_coord_t		msk_x,
				   twin_coord_t		msk_y,
				   twin_operator_t	operator,
				   twin_coord_t		width,
				   twin_coord_t		height)
{
    twin_coord_t    iy;
    twin_coord_t    left, top, right, bottom;
    twin_xform_t    *sxform = NULL, *mxform = NULL;
    twin_source_u   s;

    dst_x += dst->origin_x;
    dst_y += dst->origin_y;
    left = dst_x;
    top = dst_y;
    right = dst_x + width;
    bottom = dst_y + height;

    /* clip */
    if (left < dst->clip.left)
	left = dst->clip.left;
    if (top < dst->clip.top)
	top = dst->clip.top;
    if (right > dst->clip.right)
	right = dst->clip.right;
    if (bottom > dst->clip.bottom)
	bottom = dst->clip.bottom;

    if (left >= right || top >= bottom)
	return;

    width = right - left;
    height = bottom - top;

    if (src->source_kind == TWIN_PIXMAP) {	
	src_x += src->u.pixmap->origin_x;
	src_y += src->u.pixmap->origin_y;
	sxform = twin_pixmap_init_xform(src->u.pixmap, left, width,
					src_x, src_y);
	if (sxform == NULL)
	    return;
	s.p = sxform->span;
    } else
        s.c = src->u.argb;

    if (msk) {
	twin_src_msk_op	op;
	twin_source_u   m;
	
	if (msk->source_kind == TWIN_PIXMAP) {
	    msk_x += msk->u.pixmap->origin_x;
	    msk_y += msk->u.pixmap->origin_y;
	    mxform = twin_pixmap_init_xform(msk->u.pixmap, left, width,
					    msk_x, msk_y);
	    if (mxform == NULL)
		return;
	    m.p = mxform->span;
	} else
	    m.c = msk->u.argb;
	
	op = comp3[operator][operand_xindex(src)][operand_xindex(msk)]
		[dst->format];
	for (iy = top; iy < bottom; iy++) {
	    if (src->source_kind == TWIN_PIXMAP)
		twin_pixmap_read_xform (sxform, iy - top);
	    if (msk->source_kind == TWIN_PIXMAP)
		twin_pixmap_read_xform (mxform, iy - top);
	    (*op) (twin_pixmap_pointer (dst, left, iy), s, m, right - left);
	}
    } else {
	twin_src_op	op;
	
	op = comp2[operator][operand_xindex(src)][dst->format];	

	for (iy = top; iy < bottom; iy++) {
	     if (src->source_kind == TWIN_PIXMAP)
		twin_pixmap_read_xform (sxform, iy - top);
	    (*op) (twin_pixmap_pointer (dst, left, iy), s, right - left);
	}
    }
    twin_pixmap_damage (dst, left, top, right, bottom);
    twin_pixmap_free_xform(sxform);
    twin_pixmap_free_xform(mxform);
}

void twin_composite (twin_pixmap_t	*dst,
		     twin_coord_t	dst_x,
		     twin_coord_t	dst_y,
		     twin_operand_t	*src,
		     twin_coord_t	src_x,
		     twin_coord_t	src_y,
		     twin_operand_t	*msk,
		     twin_coord_t	msk_x,
		     twin_coord_t	msk_y,
		     twin_operator_t	operator,
		     twin_coord_t	width,
		     twin_coord_t	height)
{
    if ((src->source_kind == TWIN_PIXMAP &&
       !twin_matrix_is_identity(&src->u.pixmap->transform)) ||
        (msk && (msk->source_kind == TWIN_PIXMAP &&
		 !twin_matrix_is_identity(&msk->u.pixmap->transform))))
        _twin_composite_xform(dst, dst_x, dst_y,
				  src, src_x, src_y,
				  msk, msk_x, msk_y,
				  operator, width, height);
    else
	_twin_composite_simple(dst, dst_x, dst_y,
			       src, src_x, src_y,
			       msk, msk_x, msk_y,
			       operator, width, height);
}

static twin_argb32_t _twin_apply_alpha(twin_argb32_t v)
{
    uint16_t t1, t2, t3;
    twin_a8_t alpha = twin_get_8(v, (__BYTE_ORDER == __BIG_ENDIAN) ? 0 : 24);

    /* clear RGB data if alpha is zero */

    if (!alpha)
	return 0;

    /* twin needs ARGB format */

    if (__BYTE_ORDER == __BIG_ENDIAN)
	return alpha << 24 |
	    twin_int_mult(twin_get_8(v, 24), alpha, t1) << 16 |
	    twin_int_mult(twin_get_8(v, 16), alpha, t2) << 8 |
	    twin_int_mult(twin_get_8(v, 8), alpha, t3) << 0;

	return alpha << 24 |
	    twin_int_mult(twin_get_8(v, 0), alpha, t1) << 16 |
	    twin_int_mult(twin_get_8(v, 8), alpha, t2) << 8 |
	    twin_int_mult(twin_get_8(v, 16), alpha, t3) << 0;
}

void twin_premultiply_alpha(twin_pixmap_t *px)
{
    int x, y;
    twin_pointer_t p;

    if (px->format != TWIN_ARGB32)
	return;

    for (y = 0; y < px->height; y++) {
	p.b = px->p.b + y * px->stride;

	for (x = 0; x < px->width; x++)
	    p.argb32[x] = _twin_apply_alpha(p.argb32[x]);
    }
}

/*
 * array primary    index is OVER SOURCE
 * array secondary  index is ARGB32 RGB16 A8
 */
static twin_src_op  fill[2][3] = {
    {	/* OVER */
	_twin_c_over_a8,
	_twin_c_over_rgb16,
	_twin_c_over_argb32,
    },
    {	/* SOURCE */
	_twin_c_source_a8,
	_twin_c_source_rgb16,
	_twin_c_source_argb32,
    }
};

void
twin_fill (twin_pixmap_t    *dst,
	   twin_argb32_t    pixel,
	   twin_operator_t  operator,
	   twin_coord_t	    left,
	   twin_coord_t	    top,
	   twin_coord_t	    right,
	   twin_coord_t	    bottom)
{
    twin_src_op	    op;
    twin_source_u   src;
    twin_coord_t    iy;

    /* offset */
    left += dst->origin_x;
    top += dst->origin_y;
    right += dst->origin_x;
    bottom += dst->origin_y;

    /* clip */
    if (left < dst->clip.left)
	left = dst->clip.left;
    if (right > dst->clip.right)
	right = dst->clip.right;
    if (top < dst->clip.top)
	top = dst->clip.top;
    if (bottom > dst->clip.bottom)
	bottom = dst->clip.bottom;
    if (left >= right || top >= bottom)
	return;
    src.c = pixel;
    op = fill[operator][dst->format];
    for (iy = top; iy < bottom; iy++)
	(*op) (twin_pixmap_pointer (dst, left, iy), src, right - left);
    twin_pixmap_damage (dst, left, top, right, bottom);
}
