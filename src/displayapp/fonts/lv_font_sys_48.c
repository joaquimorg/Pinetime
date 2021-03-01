#include "lvgl/lvgl.h"

/*******************************************************************************
 * Size: 48 px
 * Bpp: 2
 * Opts: 
 ******************************************************************************/

#ifndef LV_FONT_SYS_48
#define LV_FONT_SYS_48 1
#endif

#if LV_FONT_SYS_48

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+E900 "" */
    0x0, 0x0, 0x54, 0x0, 0x0, 0x0, 0xb, 0xff,
    0xd0, 0x0, 0x0, 0x3e, 0x39, 0xf4, 0x0, 0x0,
    0xff, 0xbe, 0xfd, 0x0, 0x2, 0xff, 0xff, 0xff,
    0x0, 0x7, 0x8e, 0x38, 0xe3, 0xc0, 0xf, 0xef,
    0xbe, 0xfb, 0xd0, 0x1f, 0xff, 0xff, 0xff, 0xf0,
    0x2f, 0x8e, 0x38, 0xe3, 0xf0, 0x3f, 0xef, 0xbe,
    0xfb, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0xb3,
    0x8e, 0x38, 0xe3, 0xac, 0xbb, 0xef, 0xbe, 0xfb,
    0xec, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xe3, 0x8e,
    0x38, 0xe3, 0x8d, 0xfb, 0xef, 0xbe, 0xfb, 0xed,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0xe3, 0x8e, 0x38,
    0xe3, 0x8d, 0xfb, 0xef, 0xbe, 0xfb, 0xed, 0xbf,
    0xff, 0xff, 0xff, 0xfc, 0x77, 0x8e, 0x38, 0xe3,
    0xbc, 0x3f, 0xef, 0xbe, 0xfb, 0xf4, 0x1f, 0xff,
    0xff, 0xff, 0xf0, 0x7, 0xde, 0x38, 0xeb, 0xc0,
    0x0, 0xff, 0xbe, 0xfe, 0x0, 0x0, 0x1b, 0xff,
    0x90, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x38, 0x0,
    0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x0, 0x0,
    0x38, 0x0, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0,
    0x0, 0x0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x38,
    0x0, 0x0, 0x0, 0x0, 0xfe, 0x0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0x2, 0xff, 0x0,
    0x0, 0x0, 0x2, 0xff, 0x0, 0x0, 0x0, 0x2,
    0xff, 0x0, 0x0, 0x0, 0x2, 0xff, 0x0, 0x0,
    0x0, 0x2, 0xff, 0x0, 0x0, 0x0, 0x2, 0xff,
    0x0, 0x0, 0x0, 0x2, 0xff, 0x0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0x0, 0xfe, 0x0,
    0x0, 0x0, 0x0, 0x14, 0x0, 0x0,

    /* U+E901 "" */
    0x0, 0x0, 0x54, 0x0, 0x0, 0x0, 0xb, 0xff,
    0xd0, 0x0, 0x0, 0x3e, 0x39, 0xf4, 0x0, 0x0,
    0xff, 0xbe, 0xfd, 0x0, 0x2, 0xff, 0xff, 0xff,
    0x0, 0x7, 0x8e, 0x38, 0xe3, 0xc0, 0xf, 0xef,
    0xbe, 0xfb, 0xd0, 0x1f, 0xff, 0xff, 0xff, 0xf0,
    0x2f, 0x8e, 0x38, 0xe3, 0xf0, 0x3f, 0xef, 0xbe,
    0xfb, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0xb3,
    0x8e, 0x38, 0xe3, 0xac, 0xbb, 0xef, 0xbe, 0xfb,
    0xec, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xe3, 0x8e,
    0x38, 0xe3, 0x8d, 0xfb, 0xef, 0xbe, 0xfb, 0xed,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0xe3, 0x8e, 0x38,
    0xe3, 0x8d, 0xfb, 0xef, 0xbe, 0xfb, 0xed, 0xbf,
    0xff, 0xff, 0xff, 0xfc, 0x77, 0x8e, 0x38, 0xe3,
    0xbc, 0x3f, 0xef, 0xbe, 0xfb, 0xf4, 0x1f, 0xff,
    0xff, 0xff, 0xf0, 0x7, 0xde, 0x38, 0xeb, 0xc0,
    0x0, 0xff, 0xbe, 0xfe, 0x0, 0x0, 0x1b, 0xff,
    0x90, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x0,
    0x0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x38, 0x0,
    0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x0, 0x0,
    0x38, 0x0, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0,
    0x0, 0x0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x38,
    0x0, 0x0, 0x0, 0x0, 0xfe, 0x0, 0x0, 0x0,
    0x1, 0xcb, 0x0, 0x0, 0x0, 0x2, 0xcb, 0x0,
    0x0, 0x0, 0x2, 0xcb, 0x0, 0x0, 0x0, 0x2,
    0xcb, 0x0, 0x0, 0x0, 0x2, 0xcb, 0x0, 0x0,
    0x0, 0x2, 0xcb, 0x0, 0x0, 0x0, 0x2, 0xcb,
    0x0, 0x0, 0x0, 0x2, 0xcb, 0x0, 0x0, 0x0,
    0x1, 0xcb, 0x0, 0x0, 0x0, 0x0, 0xfe, 0x0,
    0x0, 0x0, 0x0, 0x14, 0x0, 0x0,

    /* U+E902 "" */
    0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xb, 0xff, 0xff,
    0xe0, 0x0, 0x0, 0x0, 0x0, 0x79, 0x0, 0x2f,
    0xff, 0xff, 0xf8, 0x0, 0x6d, 0x0, 0x0, 0xff,
    0xd1, 0xff, 0xff, 0xff, 0xff, 0x47, 0xff, 0x0,
    0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x3, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0xb, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xd0, 0x7,
    0xff, 0xff, 0xff, 0xfd, 0x7f, 0xff, 0xff, 0xfe,
    0x0, 0x0, 0xbf, 0xff, 0xff, 0xfd, 0xf, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0x2f, 0xff, 0xff, 0xf0,
    0x3, 0xff, 0xff, 0xf0, 0x0, 0x0, 0xf, 0xff,
    0xff, 0xc0, 0x0, 0xbf, 0xff, 0xd0, 0x0, 0x0,
    0x7, 0xff, 0xfe, 0x0, 0x0, 0x3f, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x3f,
    0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xfc, 0x0,
    0x0, 0x3f, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff,
    0xfc, 0x0, 0x0, 0x3f, 0xff, 0xc0, 0x0, 0x0,
    0x3, 0xff, 0xfc, 0x0, 0x0, 0xbf, 0xff, 0xd0,
    0x0, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x3, 0xff,
    0xff, 0xf0, 0x0, 0x0, 0xf, 0xff, 0xff, 0xc0,
    0x1f, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x2f, 0xff,
    0xff, 0xf4, 0x7f, 0xff, 0xff, 0xfe, 0x0, 0x0,
    0xbf, 0xff, 0xff, 0xfd, 0x7f, 0xff, 0xff, 0xff,
    0xd0, 0x7, 0xff, 0xff, 0xff, 0xfd, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0xb, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x0, 0xff, 0xd1, 0xff, 0xff, 0xff, 0xff, 0x47,
    0xff, 0x0, 0x0, 0x38, 0x0, 0x1f, 0xff, 0xff,
    0xf4, 0x0, 0x2c, 0x0, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0,

    /* U+E903 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xd0, 0x0, 0x0, 0xf, 0xf0, 0x0,
    0x0, 0x7, 0x40, 0x0, 0x0, 0x7, 0xf4, 0x0,
    0x0, 0xf, 0xf0, 0x0, 0x0, 0x1f, 0xd0, 0x0,
    0x0, 0xf, 0xfd, 0x0, 0x0, 0xf, 0xf0, 0x0,
    0x0, 0x7f, 0xf0, 0x0, 0x0, 0x7, 0xff, 0x40,
    0x0, 0xf, 0xf0, 0x0, 0x1, 0xff, 0xd0, 0x0,
    0x0, 0x1, 0xff, 0xd0, 0x0, 0x0, 0x0, 0x0,
    0x7, 0xff, 0x40, 0x0, 0x0, 0x0, 0x7f, 0xf4,
    0x0, 0x0, 0x0, 0x0, 0x1f, 0xfd, 0x0, 0x0,
    0x0, 0x0, 0x1f, 0xfd, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x7, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xd0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x1a, 0xff, 0xfe, 0x90,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1b,
    0xff, 0xff, 0xff, 0xff, 0x90, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x6, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xff, 0xe9, 0x0, 0x0, 0x6b,
    0xff, 0xff, 0x40, 0x0, 0x0, 0x2f, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf4, 0x0,
    0x0, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xfe, 0x0, 0x7, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xd0,
    0x1f, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xf4, 0xbf, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xfd,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0x3f, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0x2f, 0xff, 0xff, 0xfc, 0xf, 0xff, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xf0,
    0x3, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xbf, 0xff, 0xc0, 0x0, 0xff, 0xf4, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0x0,
    0x0, 0x3f, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xfc, 0x0, 0x0, 0xa, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa0, 0x0,

    /* U+E904 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xe4, 0x1, 0xbf, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xd0, 0x0,
    0x0, 0x7f, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xe0, 0x0, 0x0, 0x0, 0xbf, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0x2, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xfe,
    0x0, 0xb, 0xfe, 0x0, 0xb, 0xff, 0xc0, 0x0,
    0x0, 0x7, 0xff, 0xc0, 0xb, 0xff, 0xfe, 0x0,
    0x3f, 0xfd, 0x0, 0x0, 0x0, 0xff, 0xf4, 0x2,
    0xff, 0xff, 0xf8, 0x1, 0xff, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0x0, 0xbf, 0xff, 0xff, 0xe0, 0xf,
    0xff, 0xc0, 0x0, 0xf, 0xff, 0xe0, 0xf, 0xff,
    0xff, 0xff, 0x0, 0xbf, 0xff, 0x0, 0x3, 0xff,
    0xfd, 0x2, 0xff, 0xff, 0xff, 0xf8, 0x7, 0xff,
    0xfc, 0x0, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff,
    0xff, 0xc0, 0x3f, 0xff, 0xf0, 0x3f, 0xff, 0xfc,
    0x3, 0xff, 0xff, 0xff, 0xfc, 0x3, 0xff, 0xff,
    0xc3, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff,
    0xc0, 0x3f, 0xff, 0xfc, 0xf, 0xff, 0xfc, 0x3,
    0xff, 0xff, 0xff, 0xfc, 0x3, 0xff, 0xff, 0x0,
    0x3f, 0xff, 0xd0, 0x2f, 0xff, 0xff, 0xff, 0x80,
    0x7f, 0xff, 0xc0, 0x0, 0xff, 0xfe, 0x0, 0xff,
    0xff, 0xff, 0xf0, 0xb, 0xff, 0xf0, 0x0, 0x3,
    0xff, 0xf0, 0xb, 0xff, 0xff, 0xfe, 0x0, 0xff,
    0xfc, 0x0, 0x0, 0xf, 0xff, 0x40, 0x2f, 0xff,
    0xff, 0x80, 0x1f, 0xff, 0x0, 0x0, 0x0, 0x3f,
    0xfc, 0x0, 0xbf, 0xff, 0xe0, 0x3, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xe0, 0x0, 0xbf, 0xe0,
    0x0, 0xbf, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xc0, 0x0,
    0x0, 0x3, 0xff, 0xfe, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xfd,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xfe, 0x40, 0x1b, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,

    /* U+E905 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xe4, 0x1, 0xbf, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xd0, 0x0,
    0x0, 0x7f, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xe0, 0x0, 0x0, 0x0, 0xbf, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0x2, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0xb, 0xff, 0xc0, 0x0,
    0x0, 0x7, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xfd, 0x0, 0x0, 0x0, 0xff, 0xf4, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0xc0, 0x0, 0xf, 0xff, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xbf, 0xff, 0x0, 0x3, 0xff,
    0xfd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xfc, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0x3f, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xc3, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xfc, 0xf, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0x0,
    0x3f, 0xff, 0xd0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0xff, 0xc0, 0x0, 0xff, 0xfe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xb, 0xff, 0xf0, 0x0, 0x3,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff,
    0xfc, 0x0, 0x0, 0xf, 0xff, 0x40, 0x0, 0x0,
    0x0, 0x0, 0x1f, 0xff, 0x0, 0x0, 0x0, 0x3f,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x0, 0xbf, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xc0, 0x0,
    0x0, 0x3, 0xff, 0xfe, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xfd,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xfe, 0x40, 0x1b, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,

    /* U+E906 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xc1, 0xbf, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x7f, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0xbf, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x2, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0xb, 0xff, 0xc0, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0,
    0x3f, 0xfd, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x0, 0x1, 0xff, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0xf,
    0xff, 0xc0, 0x0, 0xf, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0xbf, 0xff, 0x0, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x7, 0xff,
    0xfc, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xfc, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x3f, 0xff, 0xfc, 0xf, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x0, 0x0, 0x3, 0xff, 0xff, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x7f, 0xff, 0xc0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0xb, 0xff, 0xf0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0xff,
    0xfc, 0x0, 0x0, 0xf, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x1f, 0xff, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x3, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0xbf, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x2f, 0xff, 0xc0, 0x0,
    0x0, 0x3, 0xff, 0xff, 0xff, 0xfc, 0x0, 0xb,
    0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff,
    0xff, 0xc0, 0x7, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xff, 0xfc, 0x1b, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,

    /* U+E907 "" */
    0x0, 0x0, 0x0, 0x3, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x90, 0x0, 0x0, 0x3f, 0xc0,
    0x0, 0x0, 0x60, 0x2f, 0x40, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1f, 0x8b, 0xfd, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xfe, 0x3f, 0xf4, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xff, 0xc1, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x2f, 0xf0, 0x7, 0xe0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xbd, 0x0, 0x18, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2, 0x40, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0,
    0x3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfc, 0x0, 0x0, 0x2f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x0, 0x0, 0xbf, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xff, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0,
    0x2f, 0xff, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0,
    0x0, 0xbf, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0xff, 0xc0, 0x0, 0x0,

    /* U+E908 "" */
    0xbf, 0xe0, 0x0, 0xbf, 0xef, 0xff, 0x0, 0xf,
    0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff, 0x0,
    0xf, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff,
    0x0, 0xf, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff,
    0xff, 0x0, 0xf, 0xff, 0xff, 0xf0, 0x0, 0xff,
    0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xf0, 0x0,
    0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xf0,
    0x0, 0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff,
    0xf0, 0x0, 0xff, 0xff, 0xff, 0x0, 0xf, 0xff,
    0xff, 0xf0, 0x0, 0xff, 0xff, 0xff, 0x0, 0xf,
    0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff, 0x0,
    0xf, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff,
    0x0, 0xf, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff,
    0xff, 0x0, 0xf, 0xff, 0xff, 0xf0, 0x0, 0xff,
    0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xf0, 0x0,
    0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xf0,
    0x0, 0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff,
    0xf0, 0x0, 0xff, 0xff, 0xff, 0x0, 0xf, 0xff,
    0xff, 0xf0, 0x0, 0xff, 0xff, 0xff, 0x0, 0xf,
    0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff, 0x0,
    0xf, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff,
    0x0, 0xf, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff,
    0xff, 0x0, 0xf, 0xff, 0xff, 0xf0, 0x0, 0xff,
    0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xf0, 0x0,
    0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff, 0xf0,
    0x0, 0xff, 0xff, 0xff, 0x0, 0xf, 0xff, 0xff,
    0xf0, 0x0, 0xff, 0xfb, 0xfe, 0x0, 0xb, 0xfe,

    /* U+E909 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xf4, 0x0, 0xf, 0xe4, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xfc, 0x0, 0xf, 0xff,
    0xd0, 0x0, 0x0, 0xff, 0xff, 0xfc, 0x0, 0xf,
    0xff, 0xfd, 0x0, 0x0, 0xff, 0xff, 0xfc, 0x0,
    0xf, 0xff, 0xff, 0x80, 0x0, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x1b, 0xff, 0xf0, 0x0, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0xbf, 0xfc, 0x0, 0xbf,
    0xff, 0xfd, 0x0, 0x0, 0x0, 0xf, 0xff, 0x0,
    0xbf, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x2, 0xff,
    0x80, 0x7f, 0xff, 0xff, 0x0, 0xf, 0x90, 0x0,
    0xbf, 0xd0, 0x3f, 0xff, 0xff, 0x0, 0xf, 0xfd,
    0x0, 0x3f, 0xf0, 0x3f, 0xff, 0xff, 0x0, 0xf,
    0xff, 0x80, 0xf, 0xf4, 0x2f, 0xff, 0xfd, 0x0,
    0xf, 0xff, 0xe0, 0xb, 0xfc, 0x1f, 0xff, 0xf4,
    0x0, 0x0, 0xbf, 0xf4, 0x3, 0xfc, 0xf, 0xff,
    0xd0, 0x0, 0x0, 0xf, 0xfc, 0x2, 0xfd, 0xb,
    0xff, 0x40, 0x0, 0x0, 0x3, 0xfd, 0x1, 0xfe,
    0x7, 0xfd, 0x0, 0x0, 0x0, 0x2, 0xfe, 0x0,
    0xff, 0x3, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff,
    0x0, 0xff, 0x1, 0xff, 0x40, 0x0, 0x0, 0x0,
    0xff, 0x0, 0xff, 0x0, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xf8,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7f, 0xf8, 0x0, 0x0, 0x7f,
    0x94, 0x0, 0x0, 0x0, 0x1f, 0xfe, 0x0, 0x1,
    0xff, 0xff, 0xfd, 0x0, 0x0, 0x7, 0xff, 0xc0,
    0x7, 0xff, 0xff, 0xff, 0x0, 0x0, 0x1, 0xff,
    0xf4, 0x1f, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x0, 0x0,
    0x0, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x2, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6f,
    0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0xbf, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xaf, 0xfd,

    /* U+E90B "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xb, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xe0, 0x0,
    0x0, 0x0, 0x0, 0xc0, 0x0, 0x3, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0xf, 0xc0, 0x0, 0xf, 0xff,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0xbf,
    0xfd, 0x0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x2f,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0xff, 0xc0,
    0x3f, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0, 0xff,
    0xc0, 0x3f, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0xff, 0xc0, 0x3f, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x2, 0xff, 0x80, 0x3f, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xf, 0xff, 0x80, 0x3f, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x7f, 0xff, 0x80, 0x7f, 0xff,
    0xfd, 0x0, 0x0, 0x2, 0xff, 0xff, 0x80, 0x7f,
    0xff, 0xf8, 0x0, 0x0, 0xf, 0xff, 0xff, 0x80,
    0x7f, 0xff, 0xf0, 0x0, 0x0, 0x3f, 0xff, 0xff,
    0x80, 0x7f, 0xff, 0xc0, 0x0, 0x0, 0xff, 0xff,
    0xff, 0x80, 0x7f, 0xff, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xff, 0x80, 0x7f, 0xfc, 0x0, 0x0, 0xf,
    0xff, 0xff, 0xff, 0x80, 0xbf, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0x80, 0xbf, 0xc0, 0x0,
    0x0, 0xff, 0xff, 0xff, 0xff, 0x80, 0xbf, 0x0,
    0x0, 0x3, 0xff, 0xff, 0xff, 0xff, 0x80, 0xbc,
    0x0, 0x0, 0xf, 0xff, 0xff, 0xff, 0xff, 0x40,
    0xb0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0x40, 0xc0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x40, 0x0, 0x0, 0xf, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x40, 0x0, 0x0, 0xbf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x40, 0x0, 0xb, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x40, 0x0, 0xbf,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40, 0xb,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xff, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0xc0, 0x0, 0x3, 0xf0, 0x0, 0x0, 0x0, 0xbf,
    0xfe, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xbe, 0x0, 0x0, 0x0, 0x0,

    /* U+E90C "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x40, 0x0, 0xb, 0xe0,
    0x0, 0x1, 0x0, 0x0, 0x0, 0x3, 0xd0, 0x0,
    0x2f, 0xf8, 0x0, 0x7, 0xc0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x3f, 0xfc, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x3f, 0xf8, 0x0, 0x3f, 0xfc, 0x0, 0x2f,
    0xfc, 0x0, 0x0, 0xff, 0xe0, 0x0, 0x7f, 0xfd,
    0x0, 0xb, 0xff, 0x0, 0x2, 0xff, 0x80, 0xb,
    0xff, 0xff, 0xe0, 0x2, 0xff, 0x80, 0x3, 0xff,
    0x0, 0x7f, 0xff, 0xff, 0xfd, 0x0, 0xff, 0xc0,
    0xf, 0xfc, 0x1, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x3f, 0xf0, 0xf, 0xf8, 0x7, 0xff, 0xff, 0xff,
    0xff, 0xd0, 0x2f, 0xf0, 0x2f, 0xf0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0xf, 0xf8, 0x3f, 0xe0,
    0x2f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xb, 0xfc,
    0x7f, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x3, 0xfd, 0xbf, 0x80, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x2, 0xfe, 0xbf, 0x40, 0xbf, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0x1, 0xfe, 0xff, 0x40,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1, 0xff,
    0xff, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xb, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xe0, 0x0, 0x0, 0x2f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x0, 0x0, 0xbf,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xb, 0xe0, 0x0, 0x0, 0x0, 0x0,

    /* U+E910 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xe0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7, 0xff, 0xff, 0x41, 0xfe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xf0, 0xff,
    0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xfc, 0xf, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0x0, 0xff, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0xff, 0xff, 0xc0, 0xf, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xf0,
    0x0, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x1, 0xff,
    0xff, 0xf8, 0x0, 0xf, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0xbf, 0xff, 0xfe, 0x0, 0x0, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0x40, 0x0,
    0xf, 0xfe, 0x0, 0x0, 0x0, 0xf, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xf0, 0x0, 0x0, 0xf, 0xfe, 0x0,
    0x0, 0x0, 0x7f, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0xff, 0xe0, 0x0, 0x0, 0x7, 0xff, 0xfd, 0x0,
    0x0, 0x0, 0xf, 0xfe, 0x0, 0x0, 0x0, 0x7f,
    0xff, 0x0, 0x0, 0x0, 0x0, 0xff, 0xe0, 0x0,
    0x0, 0x7, 0xff, 0x80, 0x0, 0x0, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x3f, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0xf, 0xfe, 0x0, 0x0,
    0x1f, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff,
    0xe0, 0x0, 0xf, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xfe, 0x0, 0xb, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xe0, 0x3, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xfe,
    0x0, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xe0, 0x2, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x5, 0xbf, 0x40, 0x0, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xf4, 0x0,
    0xf, 0xfe, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0x40, 0xb, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xf4, 0x1f, 0xff, 0xfe, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0x7f, 0xff,
    0xff, 0xe0, 0x0, 0x0, 0xf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf, 0xfe, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xfd, 0x0, 0xff, 0xe0, 0x0,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xd0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0xf, 0xff, 0xff,
    0xfe, 0x40, 0x0, 0x0, 0xf, 0xfe, 0x0, 0x3,
    0xff, 0xff, 0xe4, 0x0, 0x0, 0x0, 0x0, 0xff,
    0xe0, 0x0, 0x7f, 0xe9, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xd0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xd0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+E914 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0,
    0xff, 0x0, 0xff, 0x0, 0x0, 0xff, 0xff, 0x0,
    0x0, 0xff, 0x0, 0xff, 0x0, 0x0, 0xff, 0xff,
    0x0, 0x0, 0xff, 0x0, 0xff, 0x0, 0x0, 0xff,
    0xff, 0x0, 0x0, 0xff, 0x0, 0xff, 0x0, 0x0,
    0xff, 0xff, 0x0, 0x0, 0xff, 0x0, 0xff, 0x0,
    0x0, 0xff, 0xff, 0x0, 0x0, 0xff, 0x0, 0xff,
    0x0, 0x0, 0xff, 0xff, 0x0, 0x0, 0xff, 0x0,
    0xff, 0x0, 0x0, 0xff, 0xff, 0x0, 0x0, 0xff,
    0x0, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff,
    0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0xff,
    0xff, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff,
    0xff, 0xff, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
    0xff, 0xff, 0xff, 0xff, 0x0, 0xff, 0x0, 0xff,
    0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff,
    0x0, 0xff, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0,
    0xff, 0x0, 0xff, 0xff, 0x0, 0x0, 0xff, 0x0,
    0x0, 0xff, 0x0, 0xff, 0xff, 0x0, 0x0, 0xff,
    0x0, 0x0, 0xff, 0x0, 0xff, 0xff, 0x0, 0x0,
    0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
    0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff,
    0x0, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
    0xff, 0x0, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff,
    0x0, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0xff,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0,
    0xff, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,
    0x0, 0xff, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x0, 0xff, 0x0, 0xff
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 768, .box_w = 20, .box_h = 46, .ofs_x = 15, .ofs_y = -2},
    {.bitmap_index = 230, .adv_w = 768, .box_w = 20, .box_h = 46, .ofs_x = 15, .ofs_y = -2},
    {.bitmap_index = 460, .adv_w = 768, .box_w = 40, .box_h = 40, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 860, .adv_w = 768, .box_w = 48, .box_h = 38, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1316, .adv_w = 768, .box_w = 46, .box_h = 46, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1845, .adv_w = 768, .box_w = 46, .box_h = 46, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 2374, .adv_w = 768, .box_w = 46, .box_h = 46, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 2903, .adv_w = 768, .box_w = 34, .box_h = 40, .ofs_x = 7, .ofs_y = 0},
    {.bitmap_index = 3243, .adv_w = 384, .box_w = 18, .box_h = 48, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 3459, .adv_w = 768, .box_w = 36, .box_h = 37, .ofs_x = 6, .ofs_y = 2},
    {.bitmap_index = 3792, .adv_w = 768, .box_w = 35, .box_h = 40, .ofs_x = 7, .ofs_y = 0},
    {.bitmap_index = 4142, .adv_w = 768, .box_w = 40, .box_h = 40, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 4542, .adv_w = 768, .box_w = 41, .box_h = 41, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 4963, .adv_w = 768, .box_w = 36, .box_h = 37, .ofs_x = 6, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 0, 10, 11, 0, 0, 0,
    12, 0, 0, 0, 13
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 59648, .range_length = 21, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 21, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = gylph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
lv_font_t lv_font_sys_48 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 48,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_SYS_48*/

