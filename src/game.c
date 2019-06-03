#include "game.h"

int width = 0;
int height = 0;

float rot_cos = 0;
float rot_sin = 0;
float pitch = 0;
float fov = 0;

float x_cen = 0;
float y_cen = 0;
    
float cam_x = 0;
float cam_y = 0;
float cam_z = 0;


void init_game(Game *game){
    game->world = create_world("res/map2.png");
    game->world.camera = create_camera(game->world, game->display.bitmap->height);
    game->details.before_time = 0;
    game->details.current_time = 0;
    game->details.delta_time = 0;
    game->details.fps_time = 0;
    game->details.fps_count = 0;
    game->details.fps_current = 0;
}

void handle_input_mouse(Game *game, int x_rel){
    float dt = game->details.delta_time;
    float rot_speed = (x_rel * 0.0025f) * dt; 
    
   // game->world.camera.rot += rot_speed;
}

void handle_input(Game *game){
    
    float dt = game->details.delta_time;

    float rot_speed = 0.0025f * dt; 
    float move_speed = 0.0025f * dt; 
    
    float fX = sin(game->world.camera.rot);
    float fY = cos(game->world.camera.rot);

    float sX = cos(game->world.camera.rot);
    float sY = -sin(game->world.camera.rot);

    u8* keys = game->input.keys;

    if (keys[SDL_SCANCODE_W]){
        game->world.camera.x += move_speed * fX;
        game->world.camera.y += move_speed * fY;
    }
    if (keys[SDL_SCANCODE_A]){
        game->world.camera.x -= move_speed * sX;
        game->world.camera.y -= move_speed * sY;
    }
    if (keys[SDL_SCANCODE_S]){
        game->world.camera.x -= move_speed * fX;
        game->world.camera.y -= move_speed * fY;
    }
    if (keys[SDL_SCANCODE_D]){
        game->world.camera.x += move_speed * sX;
        game->world.camera.y += move_speed * sY;
    }
    if (keys[SDL_SCANCODE_Q]){
        game->world.camera.rot -= rot_speed;
    }
    if (keys[SDL_SCANCODE_E]){
        game->world.camera.rot += rot_speed; 
    }
    if (keys[SDL_SCANCODE_SPACE]){
        game->world.camera.z += 0.0025 * dt;
    }
   // if (keys[SDL_SCANCODE_LSHIFT]){
   //     game->world.camera.z -= 0.0025 * dt;
   // }
    if (keys[SDL_SCANCODE_O]){
        game->world.camera.pitch += 0.0025 * dt;
    }
    if (keys[SDL_SCANCODE_P]){
        game->world.camera.pitch -= 0.0025 * dt; 
    }
    if (keys[SDL_SCANCODE_ESCAPE]){
        SDL_SetRelativeMouseMode(false);
    }
    if (keys[SDL_SCANCODE_K]){
        create_entity(&game->world, game->world.camera.x, game->world.camera.y, 1.0f, 3.0f, 0);
    }
}

void render_pre_process(Game *game){
    width = game->display.bitmap->width;
    height = game->display.bitmap->height;

    rot_cos = cos(game->world.camera.rot);
    rot_sin = sin(game->world.camera.rot);
    pitch = game->world.camera.pitch;
    fov = game->world.camera.fov;

    x_cen = 0.5f * width;
    y_cen = (0.5f + pitch) * height;
    
    cam_x = game->world.camera.x;
    cam_y = game->world.camera.y;
    cam_z = game->world.camera.z;
}

void render_wall(Game *game, float x0, float y0, float x1, float y1, int tex_num){
    
    float x_cen_start = 2 * (x0 - cam_x) - 1.0f;
	float y_cen_start = 2 * (y0 - cam_y) - 1.0f;

    float x_cen_end = 2 * (x1 - cam_x) - 1.0f;
	float y_cen_end = 2 * (y1 - cam_y) - 1.0f;

	float x_start = x_cen_start * rot_cos - y_cen_start * rot_sin;
	float z_start = y_cen_start * rot_cos + x_cen_start * rot_sin;
	
    float x_end = x_cen_end * rot_cos - y_cen_end * rot_sin;
	float z_end = y_cen_end * rot_cos + x_cen_end * rot_sin;

    float upper_pix  = 2 * cam_z - 1;
	float lower_pix  = 2 * cam_z + 1;

	float x_tex_start = 0;
	float x_tex_end = TILE_SIZE;

	float depth_clip = 0.001f;
	if (z_start < depth_clip && z_end < depth_clip){
		return;
    }

	if (z_start < depth_clip) {
		float p = (depth_clip - z_start) / (z_end - z_start);
		z_start = lerp(z_start, z_end, p);
		x_start = lerp(x_start, x_end, p);
		x_tex_start = lerp(x_tex_start, x_tex_end, p);
	}

	if (z_end < depth_clip) {
		float p = (depth_clip - z_start) / (z_end - z_start);
		z_end = lerp(z_start, z_end, p);
		x_end = lerp(x_start, x_end, p);
		x_tex_end = lerp(x_tex_start, x_tex_end, p);
	}

	float x_pix_start = (x_start / z_start / fov) + x_cen;
	float x_pix_end = (x_end / z_end / fov) + x_cen;
	if (x_pix_start >= x_pix_end){
		return;
    }

	int ix_pix_start = (int)ceil(x_pix_start);
	int ix_pix_end = (int)ceil(x_pix_end);

	if (ix_pix_start < 0)
		ix_pix_start = 0;
	if (ix_pix_end > width)
		ix_pix_end = width;

	float y_pix_start0 = (upper_pix / z_start / fov) + y_cen;
	float y_pix_end0 = (lower_pix / z_start / fov) + y_cen;

	float y_pix_start1 = (upper_pix / z_end / fov) + y_cen;
	float y_pix_end1 = (lower_pix / z_end / fov) + y_cen;

	float fz_start = 1.0f / (float)z_start;
	float fz_end = 1.0f / (float)z_end;
    
    float fx_pix = 1.0f / (x_pix_end - x_pix_start);

	for (int x = ix_pix_start; x < ix_pix_end; x++) {
		float p_x = (x - x_pix_start) * fx_pix;

		float depth = lerp(fz_start, fz_end, p_x);

		int tex_x = lerp(x_tex_start * fz_start, x_tex_end * fz_end, p_x) / depth;

		float y_pix_start = lerp(y_pix_start0, y_pix_start1, p_x);
		float y_pix_end = lerp(y_pix_end0, y_pix_end1, p_x);

		int iy_pix_start = (int)ceil(y_pix_start);
		int iy_pix_end = (int)ceil(y_pix_end);

		if (iy_pix_start < 0){
			iy_pix_start = 0;
        }
		if (iy_pix_end > height){
			iy_pix_end = height;
        }

        float fy_pix = 1.0f / (y_pix_end - y_pix_start);

		for (int y = iy_pix_start; y < iy_pix_end; y++) {
			float p_y = (y - y_pix_start) * fy_pix;
			int tex_y = (int)ceil(p_y * TILE_SIZE);

			if (game->display.depth_buffer[x + y * width] > depth){
				continue;
            }

		    Pixel pixel = get_pixel(game->res.art.walls,
			                       (tex_x % TILE_SIZE) + (tex_num % TILE_SIZE) * TILE_SIZE,
			                       (tex_y % TILE_SIZE) + (tex_num / TILE_SIZE) * TILE_SIZE);
			if (pixel.a <= 0){
                continue;                
            }

			game->display.bitmap->pixels[x + y * width] = pixel;
			game->display.depth_buffer[x + y * width] = depth;
		}
	}
}

void render_walls(Game *game){
    int r = 10;
    int x_cen = game->world.camera.x;
    int y_cen = game->world.camera.y;

    for (int y = y_cen - r; y <= y_cen + r; y++){
        for (int x = x_cen - r; x <= x_cen + r; x++){
            Tile current = get_tile(game->world, x, y);
            Tile north = get_tile(game->world, x, y - 1);
            Tile east = get_tile(game->world, x + 1, y);
            Tile south = get_tile(game->world, x, y + 1);
            Tile west = get_tile(game->world, x - 1, y);
            
            if (north.wall){
                render_wall(game, x + 1, y, x, y, north.wall_tex);
            }

            if (east.wall){
                render_wall(game, x + 1, y + 1, x + 1, y, east.wall_tex);
            }

            if (south.wall){
                render_wall(game, x, y + 1, x + 1, y + 1, south.wall_tex);
            }

            if (west.wall){
                render_wall(game, x, y, x, y + 1, west.wall_tex);
            }
        }
    }
}


void render_floor(Game *game){

    Pixel* pix_row = game->display.bitmap->pixels - width;

    for (int y = 0; y < height; y++){
        pix_row += width;
        float y_fov = ((y + 0.5f) - y_cen) * fov;
        float z_pos = 0;
        bool is_ceil = false;

        if (y_fov > 0){
            z_pos = TILE_SIZE * (cam_z + 0.5f) / y_fov;
        }
        else{
            z_pos = TILE_SIZE * (cam_z - 0.5f) / y_fov;
            is_ceil = true;
        }

        float depth = 1.0f / z_pos;

        for (int x = 0; x < width; x++){
            if (game->display.depth_buffer[x + y * width] > depth){
                continue;
            }

            float x_fov = z_pos * (x - x_cen) * fov;

            float x_pos = (x_fov * rot_cos + z_pos * rot_sin) + ((cam_x + 0.5f) * TILE_SIZE);
            float y_pos = (z_pos * rot_cos - x_fov * rot_sin) + ((cam_y + 0.5f) * TILE_SIZE);

            int ix_pos = (int)x_pos;
            int iy_pos = (int)y_pos;

            int x_tile = ix_pos / TILE_SIZE;
            int y_tile = iy_pos / TILE_SIZE;
            Tile tile = get_tile(game->world, x_tile, y_tile); 
             
            if (x_pos < 0){
                ix_pos--;
            }
            if (y_pos < 0){
                iy_pos--;
            }

            int tex_num = 0;
            int x_tex_offset = ix_pos & (TILE_SIZE - 1);
            int y_tex_offset = iy_pos & (TILE_SIZE - 1);
            
            if (x_tile >= 0 && x_tile < game->world.width && y_tile >= 0 
                    && y_tile < game->world.height){

                if (!is_ceil){
                    tex_num = tile.floor_tex;
                }
                else{
                    tex_num = tile.ceil_tex;
                }

            }

            int x_tex = (tex_num % TILE_SIZE) * TILE_SIZE;
            int y_tex = (tex_num / TILE_SIZE) * TILE_SIZE;

            int x_tex_final = x_tex + x_tex_offset;
            int y_tex_final = y_tex + y_tex_offset;
            
            if (tex_num <= -1){
                game->display.depth_buffer[x + y * width] = -1;
            }
            else{
                Pixel pixel = get_pixel(game->res.art.floors, x_tex_final, y_tex_final);
                pix_row[x] = pixel;
                game->display.depth_buffer[x + y * width] = (TILE_SIZE / 2) * depth;
            }
        }
    }
}

void render_sprite(Game *game, float x, float y, float z, float scale, int tex_num){
    float x_cen_start = (cam_x - x) * 2;
    float y_cen_start = (cam_y - y) * -2;
    float z_cen_start = (cam_z - z) * 2;

	float x_start = x_cen_start * rot_cos + y_cen_start * rot_sin;
	float z_start = y_cen_start * rot_cos - x_cen_start * rot_sin;

    if (z_start < 0.001f){
        return;
    }

    float fov_depth = 1.0f / (fov * z_start);
    
    float x_pix = x_cen - x_start * fov_depth;
    float y_pix = y_cen + z_cen_start * fov_depth;

    float x_pix_start = x_pix - scale * fov_depth;
    float x_pix_end = x_pix + scale * fov_depth;

    float y_pix_start = y_pix - scale * fov_depth;
    float y_pix_end = y_pix + scale * fov_depth;

    int ix_pix_start = (int)clamp(ceil(x_pix_start), 0, width);
    int ix_pix_end = (int)clamp(ceil(x_pix_end), 0, width);

    int iy_pix_start = (int)clamp(ceil(y_pix_start), 0, height);
    int iy_pix_end = (int)clamp(ceil(y_pix_end), 0, height);

    float depth = 1.0f / z_start;

    for (int sy = iy_pix_start; sy < iy_pix_end; sy++){
        float tex_sy = ((sy - y_pix_start) / (y_pix_end - y_pix_start)) * TILE_SIZE;
        int tex_y = tex_sy + ((tex_num / TILE_SIZE) * TILE_SIZE);

        for (int sx = ix_pix_start; sx < ix_pix_end; sx++){
            if (game->display.depth_buffer[sx + sy * width] > depth){
                continue;
            }

            float tex_sx = ((sx - x_pix_start) / (x_pix_end - x_pix_start)) * TILE_SIZE;
            int tex_x = tex_sx + ((tex_num % TILE_SIZE) * TILE_SIZE);
            
            Pixel pixel = get_pixel(game->res.art.sprites, tex_x, tex_y);
            if (pixel.a == 0){
                continue;
            }

            game->display.bitmap->pixels[sx + sy * width] = pixel;
            game->display.depth_buffer[sx + sy * width] = depth;
        } 
    } 

}

void render_entities(Game *game){
    for (int i = 0; i < game->world.entity_count; i++){
        Entity *e = &game->world.entities[i];
        
        float distance = (float)dist(e->x, e->y, e->z, cam_x, cam_y, cam_z);
        
        if (distance > 11){
            continue;
        }

        render_sprite(game, e->x, e->y, e->z, e->scale, e->tex_num);

    }
    
}

void render_ui_text(Game *game, char *text, float screen_x, float screen_y){
    int text_length = strlen(text);

    for (int i = 0; i < text_length; i++){
        int character_index = get_character_index(text[i]);
        if (character_index <= -1){
            continue;
        }
        int sprite_x = character_index % TILE_SIZE; 
        int sprite_y = character_index / TILE_SIZE;
        for (int y = 0; y < CHARACTER_HEIGHT; y++){
            for (int x = 0; x < CHARACTER_WIDTH; x++){
                
                Pixel pixel = get_pixel(game->res.art.font, 
                        sprite_x * CHARACTER_WIDTH + x,
                        sprite_y * CHARACTER_HEIGHT + y);
                
                if (pixel.a == 0){
                    continue;
                }

                set_pixel(game->display.bitmap, pixel, 
                        x + screen_x + i * CHARACTER_WIDTH, y + screen_y);
                        
            }
        }
    }
}

void render_ui_item(Game *game, float screen_x, float screen_y, float item_x, float item_y,
        float item_width, float item_height){
    
    for (int y = item_y; y < item_y + item_height; y++){
        for (int x = item_x; x < item_x + item_width; x++){
            Pixel pixel = get_pixel(game->res.art.ui, x, y);

            if (pixel.a == 0){
                continue;
            }

            int render_x = screen_x + x - item_x;
            int render_y = screen_y + y - item_y;

            set_pixel(game->display.bitmap, pixel, render_x, render_y); 
        }
    }

}

void render_post_process(Game *game){
    for (int i = 0; i < width * height; i++){
        
        float depth = game->display.depth_buffer[i];
        
        if (depth <= -1){
            int x_start = floor((int)(i % width) - game->world.camera.rot 
                    * game->res.art.skybox->width / 10.0f);
            int x = x_start & game->res.art.skybox->width-1;
            int y = i / width;
            Pixel sky_pixel = get_pixel(game->res.art.skybox, x, y);

            game->display.bitmap->pixels[i] = sky_pixel;
            continue;
        }        

        Pixel pixel = game->display.bitmap->pixels[i];
        if (pixel.a == 0){
            continue;
        }

        float fog_amount = 0.1f;
        float fog = exp(-fog_amount/depth);
        fog = clamp(fog, 0, 1);
     
        Pixel new_pixel = pixel;
        new_pixel.r *= fog;
        new_pixel.g *= fog;
        new_pixel.b *= fog;
         
        game->display.bitmap->pixels[i] = new_pixel;
    }
}

void update_game(Game *game){
    handle_input(game); 
}

void render_game(Game *game){
    render_pre_process(game);
    render_floor(game);
    render_walls(game);
    render_entities(game);
    render_post_process(game);
    
    render_ui_text(game, "Welcome adventurer!", 15, 25);
    render_ui_item(game, 20, 5, 8, 0, 8, 8);
    //render_ui_item(game, 20, 18, 11, 10, 11, 10);
    // ui here
}
