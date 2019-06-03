#ifndef BITMAP_H
#define BITMAP_H

#include "common.h"

#define PIXEL_BYTE_SIZE 4

typedef struct Pixel{
    u8 r, g, b, a;
} Pixel;

typedef struct Bitmap{
    int width;
    int height;
    int pitch;
    Pixel *pixels;

} Bitmap;

static Pixel PIXEL_TRANSPARENT = {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00};

Bitmap *load_bitmap(const char* file);
Bitmap *create_bitmap(int width, int height);
void destroy_bitmap(Bitmap *bitmap);

void set_pixel(Bitmap *bitmap, Pixel pixel, int x, int y);
Pixel get_pixel(Bitmap *bitmap, int x, int y);

void draw_bitmap_on_bitmap(Bitmap *source, Bitmap *target, int x, int y);

#endif
