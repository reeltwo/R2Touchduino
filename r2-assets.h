// This file was generated with the command-line:
//    /home/mimir/.local/bin/gd2asset -f r2.gd2 -o r2-assets.h r2d2-image-small2.png R2.PNG Dome.png

#define R2D2_IMAGE_SMALL2_HANDLE 0
#define R2D2_IMAGE_SMALL2_WIDTH 173
#define R2D2_IMAGE_SMALL2_HEIGHT 272
#define R2D2_IMAGE_SMALL2_CELLS 1
#define R2_HANDLE 1
#define R2_WIDTH 37
#define R2_HEIGHT 41
#define R2_CELLS 1
#define DOME_HANDLE 2
#define DOME_WIDTH 185
#define DOME_HEIGHT 194
#define DOME_CELLS 1
#define ASSETS_END 168926UL
#define LOAD_ASSETS()  (GD.safeload("r2.gd2"), GD.loadptr = ASSETS_END)
static const shape_t R2D2_IMAGE_SMALL2_SHAPE = {0, 173, 272, 0};
static const shape_t R2_SHAPE = {1, 37, 41, 0};
static const shape_t DOME_SHAPE = {2, 185, 194, 0};
struct {
  Bitmap r2d2_image_small2;
  Bitmap r2;
  Bitmap dome;
} bitmaps = {
 /* r2d2_image_small2 */  {{173, 272}, { 86, 136},      0x0UL,  6,  0},
 /*               r2 */  {{ 37,  41}, { 18,  20},  0x16fa0UL,  6,  1},
 /*             dome */  {{185, 194}, { 92,  97},  0x17b7aUL,  6,  2}
};
