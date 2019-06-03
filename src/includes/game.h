#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "bitmap.h"
#include "resources.h"
#include "world.h"
#include <math.h>
#define RES_WIDTH 320
#define RES_HEIGHT 240
#define RES_SCALE 4


typedef struct Framebuffer{
    SDL_Window *window; 
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Bitmap *bitmap;
    float *depth_buffer;
} Framebuffer;

typedef struct Input{
    u8* keys;
} Input;

typedef struct Game_Details{
    bool running;
    bool focused;
    u32 before_time;
    u32 current_time;
    u32 delta_time;
    u32 fps_time;
    u32 fps_count;
    u32 fps_current;
} Game_Details;

typedef struct Game{
    Game_Details details;
    Framebuffer display;
    Input input;
    Resource res;
    World world;
} Game;

Game *create_game(void);
void destroy_game(Game *game);
void init_game(Game *game);

void handle_input(Game *game);
void update_game(Game *game);
void render_floor(Game *game);
void render_wall(Game *game, float x0, float y0, float x1, float y1, int tex_num);
void render_sprite(Game *game, float x, float y, float z, float scale, int tex_num); 
void render_ui_item(Game *game, float screen_x, float screen_y, float item_x, float item_y,
        float item_width, float item_height);
void render_walls(Game *game);
void render_game(Game *game);
void render_pre_process(Game *game);
void render_post_process(Game *game);


#endif
