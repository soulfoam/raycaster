#include "world.h"
#include "bitmap.h"

Camera create_camera(World world, int height){
    Camera camera;
    camera.x = world.start_x;
    camera.y = world.start_y;
    camera.z = 0.1f;
    camera.rot = 1.5f;
    camera.pitch = -0.1f;
    camera.fov = 1 / (float) height;
    return camera;
}

Entity *create_entity(World *world, float x, float y, float z, float scale, int tex_num){
    if (world->entity_count >= MAX_ENTITY_SIZE){
        printf("Gee wilikers boys, we are FUCKED. Call the command station!\n");
        return NULL;
    }
 
    Entity entity;
    entity.id = world->entity_count;
    entity.x = x;
    entity.y = y;
    entity.z = z;
    entity.scale = scale;
    entity.tex_num = tex_num;

    world->entities[world->entity_count] = entity;
    
    world->entity_count++;

    return &world->entities[world->entity_count]; 
}

void destroy_entity(World *world, int entity_id){
    if (world->entity_count > 0 && entity_id < world->entity_count && entity_id >= 0){
        int previous_entity = world->entity_count - 1;
        for (int i = entity_id; i < previous_entity; i++){
            world->entities[i] = world->entities[i + 1];
        }
        world->entity_count--;
    } 
}

World create_world(char *file_name){
    World world;
    Bitmap *map_file = load_bitmap(file_name);
    world.width = map_file->width;
    world.height = map_file->height;
    world.tiles = calloc(world.width * world.height, sizeof(Tile));
    world.start_x = 0;
    world.start_y = 0;
    world.entity_count = 0;
    
    for (int y = 0; y < world.height; y++){
        for (int x = 0; x < world.width; x++){
            Tile *t = &world.tiles[x + y * world.width];
            t->floor_tex = 1;
            t->ceil_tex = 0;
            t->wall_tex = 0;
            t->wall = false;
        }
    }

    srand(time(NULL));

    for (int y = 0; y < world.height; y++){
        for (int x = 0; x < world.width; x++){
            Tile *t = &world.tiles[x + y * world.width];
            Pixel p = get_pixel(map_file, x, y);
             
            if (p.r == 255 && p.b == 0){
                t->wall = true;
            }
            if (p.g == 255 && p.b == 0){
                t->wall = true;
                t->wall_tex = 1;
            }
            if (p.b == 255 && p.r == 0){
                t->wall = true;
                t->wall_tex = 2;
            }
            if (p.r == 255 && p.g == 255 && p.b == 255){
                world.start_x = x;
                world.start_y = y;
            }
            if (p.r == 100 && p.g == 100 && p.b == 100){
                t->wall = false;
                t->ceil_tex = -1;
            }

        }
    } 
   
    return world;
}

Tile get_tile(World world, float x, float y){
    Tile tile;
    
    tile.ceil_tex = 0;
    tile.floor_tex = 1;

    int xx = (int)floor(x);
    int yy = (int)floor(y);
    
    if (xx < 0 || xx >= world.width || yy < 0 || yy >= world.height){
        return tile; 
    }

    return world.tiles[xx + yy * world.width];
}

void set_tile(World *world, Tile tile, float x, float y){
    int xx = (int)floor(x);
    int yy = (int)floor(y);

    if (xx < 0 || xx >= world->width || yy < 0 || yy >= world->height){
        return; 
    }
    
    world->tiles[xx + yy * world->width] = tile;
}
