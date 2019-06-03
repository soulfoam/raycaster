#ifndef RESOURCES_H
#define RESOURCES_H

#include "bitmap.h"

typedef struct Art{
    Bitmap *floors;
    Bitmap *walls;
    Bitmap *sprites;
    Bitmap *ui;
    Bitmap *font;
    Bitmap *skybox;
} Art;

typedef struct Resource{
    Art art;
} Resource;

Resource create_resources(void);

#endif
