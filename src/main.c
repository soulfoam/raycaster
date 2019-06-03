#include "game.h"

Resource create_resources(void){
    Resource resource; 
    resource.art.floors = load_bitmap("res/floors.png");
    resource.art.walls = load_bitmap("res/walls.png");
    resource.art.sprites = load_bitmap("res/sprites.png");
    resource.art.ui = load_bitmap("res/ui.png");
    resource.art.font = load_bitmap("res/font.png");
    resource.art.skybox = load_bitmap("res/skybox.png");
    return resource;
}

Game *create_game(void){
 
    Game *game = malloc(sizeof(Game));
    
    game->res = create_resources();

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL failed to init!");
        return NULL;
    }
   
    printf("[SDL] SDL initialized!\n"); 

    game->display.window = SDL_CreateWindow("LowRez", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, RES_WIDTH*RES_SCALE, RES_HEIGHT*RES_SCALE, 
            SDL_WINDOW_SHOWN);

    if (game->display.window == NULL){
        printf("Window failed to init! Error: %s\n", SDL_GetError());
        return NULL;
    }
    
    printf("[SDL] Window initialized!\n");

    game->display.renderer = SDL_CreateRenderer(game->display.window, -1, 
            SDL_RENDERER_SOFTWARE);
    
    if (game->display.renderer == NULL){
        printf("Renderer failed to init! Error %s\n", SDL_GetError());
        return NULL;
    }
    
    printf("[SDL] Renderer initialized!\n");

    SDL_RenderSetLogicalSize(game->display.renderer, RES_WIDTH, RES_HEIGHT);
    
    game->display.texture = SDL_CreateTexture(game->display.renderer, SDL_PIXELFORMAT_RGBA32, 
            SDL_TEXTUREACCESS_STREAMING, RES_WIDTH, RES_HEIGHT);

    if (game->display.texture == NULL){
        printf("Texture failed to init! Error: %s\n", SDL_GetError());
        return NULL;
    }

    game->display.bitmap = create_bitmap(RES_WIDTH, RES_HEIGHT);
    
    if (game->display.bitmap == NULL){
        printf("Screen bitmap failed to init!");
        return NULL;
    }
    
    game->display.depth_buffer = 
        calloc(game->display.bitmap->width * game->display.bitmap->height, sizeof(float));

    game->details.running = true;

    printf("[SDL] Texture initialized!\n");

    printf("[Game] Game initialized!\n"); 

    return game; 
}

void destroy_game(Game *game){
    SDL_DestroyWindow(game->display.window);
    game->display.window = NULL;
    SDL_Quit();
    printf("Destroying game and exiting...");
    exit(0);
}

void handle_events(Game *game){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_WINDOWEVENT:
                switch(event.window.event){
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        SDL_SetRelativeMouseMode(true);
                        game->details.focused = false;
                    break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        SDL_SetRelativeMouseMode(true);
                        game->details.focused = true;
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                if (event.motion.xrel){
                    handle_input_mouse(game, event.motion.xrel);
                }
                break;
            case SDL_QUIT:
                destroy_game(game);
                break;
            default:
                break;
        }
    }
}

void clear_display(Game *game){
    memset(game->display.depth_buffer, 0, 
            game->display.bitmap->width * game->display.bitmap->height * sizeof(float));
    
    for (int i = 0; i < game->display.bitmap->width * game->display.bitmap->height; i++){
        game->display.bitmap->pixels[i] = PIXEL_TRANSPARENT;
    } 
}

void render(Game *game){
    clear_display(game);
    render_game(game);
    SDL_UpdateTexture(game->display.texture, NULL, game->display.bitmap->pixels, 
            game->display.bitmap->pitch);
    SDL_RenderClear(game->display.renderer); 
    SDL_RenderCopy(game->display.renderer, game->display.texture, NULL, NULL);
    SDL_RenderPresent(game->display.renderer);
}

void main_loop(Game *game){
    while (game->details.running){ 
        handle_events(game);
        
        u32 now_time = SDL_GetTicks();
        if (now_time > game->details.current_time + 1000){
            printf("fps: %i x: %f y %f\n", 
                    game->details.fps_count, game->world.camera.x, game->world.camera.y);
            game->details.current_time = now_time;
            game->details.fps_count = 0;
        }

        game->details.delta_time = now_time - game->details.before_time;
        game->details.before_time = now_time;
 
        game->input.keys = SDL_GetKeyboardState(NULL);
        update_game(game);
        render(game);

        game->details.fps_count++;

    }
}

int main(int argc, char **argv){
      
    Game *game = create_game();
    
    if (game == NULL){
        printf("Game failed to init.. exiting...");
        exit(1);
    }
     
    init_game(game);

    main_loop(game);

    destroy_game(game);
    
    return 0;
}


