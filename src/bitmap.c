#include "bitmap.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Bitmap *create_bitmap(int width, int height){
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    bitmap->width = width;
    bitmap->height = height;
    bitmap->pitch = sizeof(Pixel) * width;
    bitmap->pixels = malloc(bitmap->pitch * height);
    return bitmap;
}

void destroy_bitmap(Bitmap *bitmap){
    free(bitmap->pixels); 
    free(bitmap);
}

void set_pixel(Bitmap *bitmap, Pixel pixel, int x, int y){
    if (x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height){
        return;
    }

    bitmap->pixels[x + y * bitmap->width] = pixel;
}

Pixel get_pixel(Bitmap *bitmap, int x, int y){
    if (x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height){
        return PIXEL_TRANSPARENT;
    }
     
    return bitmap->pixels[x + y * bitmap->width];
}

Bitmap *load_bitmap(const char* file){
    Bitmap *bitmap = malloc(sizeof(Bitmap));

    u8* pixels = stbi_load(file, &bitmap->width, &bitmap->height, NULL, PIXEL_BYTE_SIZE);
    
    if (pixels == NULL){
        printf("Couldn't load pixels from file \"%s\" \n", file);
        return NULL;
    }
    
    u32 num_of_bytes = bitmap->width * bitmap->height * PIXEL_BYTE_SIZE;
    bitmap->pixels = malloc(num_of_bytes);

    memcpy(bitmap->pixels, pixels, num_of_bytes);

    stbi_image_free(pixels);

    return bitmap;
}

void draw_bitmap_on_bitmap(Bitmap *source, Bitmap *target, int x, int y){
    if (source == NULL || target == NULL || source->width == 0 || source->height == 0 
            || target->width == 0 || target->height == 0 || x < 0 || y < 0){
        return;
    }

    for (int yy = 0; yy < source->height; yy++){
        for (int xx = 0; xx < source->width; xx++){
            Pixel pixel = get_pixel(source, xx, yy);
            if (pixel.a > 0){
                set_pixel(target, pixel, x, y);
            }
        }
    } 
}
