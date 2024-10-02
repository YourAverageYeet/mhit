#include "base-sdl.h"

vis_t* generateSDLVisualizer(void){
    srand(time(NULL));
    int rFlags = SDL_RENDERER_ACCELERATED;
    int wFlags = SDL_WINDOW_SHOWN;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Couldn't initialize SDL:\n\t%s\n\n", SDL_GetError());
        exit(1);
    }
    vis_t* vis = malloc(sizeof(vis_t));
    if(vis == NULL){
        errorOut(noMem, EC_noMem);
    }
    vis->wind = SDL_CreateWindow("MHIT Visualizer", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, SCR_W, SCR_H, wFlags);
    if(vis->wind == NULL){
        printf("SDL_CreateWindow failure:n\n\t%s\n\n", SDL_GetError());
        exit(1);
    }
    SDL_Surface* icn_surf = SDL_CreateRGBSurface(0, gsv_icon.width,
                            gsv_icon.height, (gsv_icon.bytes_per_pixel * 8),
                            GIMP_RED, GIMP_GREEN, GIMP_BLUE, GIMP_ALPHA);
    unsigned char pixels[16384] = {0};
    for(int i = 0; i < 5; i++){
        memcpy((pixels + gsv_icon.block_offsets[i]),
            gsv_icon.pixel_data_blocks[i], gsv_icon.block_sizes[i]);
    }
    icn_surf->pixels = pixels;
    SDL_SetWindowIcon(vis->wind, icn_surf);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    vis->rend = SDL_CreateRenderer(vis->wind, -1, rFlags);
    if(vis->rend == NULL){
        printf("SDL_CreateRenderer failure:n\n\t%s\n\n", SDL_GetError());
        exit(1);
    }
    return vis;
}

void destroySDLVisualizer(vis_t* vis){
    SDL_DestroyRenderer(vis->rend);
    SDL_DestroyWindow(vis->wind);
    free(vis);
}

void handleSDLInput(int* eVar){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case(SDL_QUIT):
                    *eVar = 1;
                    break;
                default:
                    break;
            }
    }
}

void showScreen(vis_t* vis){
    SDL_RenderPresent(vis->rend);
}

void placePixel(vis_t* vis, objPos_t pos, uint8_t r, uint8_t g, uint8_t b){
    SDL_SetRenderDrawColor(vis->rend, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(vis->rend, pos.x_pos, pos.y_pos);
}

void drawBlock(vis_t* vis, objPos_t pos, uint16_t w, uint16_t h, uint8_t r,
                uint8_t g, uint8_t b){
    SDL_SetRenderDrawColor(vis->rend, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_Rect block = {0, 0, 0, 0};
    block.x = pos.x_pos;
    block.y = pos.y_pos;
    block.w = w;
    block.h = h;
    SDL_RenderFillRect(vis->rend, &block);
}

// TESTING FUNCTIONS

void renderTestColor(vis_t* vis){
    SDL_SetRenderDrawColor(vis->rend, 255, 135, 92, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(vis->rend);
}

void drawTestBox(vis_t* vis){
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    SDL_SetRenderDrawColor(vis->rend, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_Rect box = {0, 0, 0, 0};
    box.w = rand() % (int)(SCR_W / 2);
    box.h = rand() % (int)(SCR_H / 2);
    box.x = rand() % (SCR_W - box.w);
    box.y = rand() % (SCR_H - box.h);
    SDL_RenderDrawRect(vis->rend, &box);
}

void drawTestLine(vis_t* vis){
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    SDL_SetRenderDrawColor(vis->rend, r, g, b, SDL_ALPHA_OPAQUE);
    uint16_t x2 = rand() % SCR_W;
    uint16_t y2 = rand() % SCR_H;
    uint16_t x1 = rand() % SCR_W;
    uint16_t y1 = rand() % SCR_H;
    SDL_RenderDrawLine(vis->rend, x1, y1, x2, y2);
}

void drawTestPoint(vis_t* vis){
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    SDL_SetRenderDrawColor(vis->rend, r, g, b, SDL_ALPHA_OPAQUE);
    uint16_t x = rand() % SCR_W;
    uint16_t y = rand() % SCR_H;
    SDL_RenderDrawPoint(vis->rend, x, y);
}
