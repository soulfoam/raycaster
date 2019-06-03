#ifndef WORLD_H
#define WORLD_H

#include "common.h"

#define TILE_SIZE 16

#define MAX_ENTITY_SIZE 256

typedef struct Camera{
    float x, y, z;
    float rot, pitch, fov;
} Camera;

typedef struct Entity{
    int id, tex_num;
    float x, y, z, scale;
} Entity;

typedef struct Tile{
    s8 floor_tex;
    s8 ceil_tex;
    s8 wall_tex;
    bool wall;
} Tile;

typedef struct World{
    int width, height;
    Tile *tiles;
    int entity_count;
    Camera camera;
    Entity entities[MAX_ENTITY_SIZE];
    int start_x, start_y;
} World;

Camera create_camera(World world, int height);
Entity *create_entity(World *world, float x, float y, float z, float scale, int tex_num);
void destroy_entity(World *world, int entity_id);
World create_world(char *file_name);
Tile get_tile(World world, float x, float y);
void set_tile(World *world, Tile tile, float x, float y);

#endif
