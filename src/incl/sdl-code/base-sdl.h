/**
 * @file base-sdl.h
 * @author Mike Hensley (mike2@tamu.edu)
 * @brief Low-level SDL Code for window generation. Heavily based on the
 * tutorial at Parallel Realities, with some info from Lazy Foo' Productions
 * @version 0.1
 * @date 2024-05-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <SDL2/SDL.h>

#define SCR_W 640
#define SCR_H 480

typedef struct vis_s {
    SDL_Renderer*   rend;
    SDL_Window*     wind;
} vis_t;

vis_t* generateSDLVisualizer(void);

void destroySDLVisualizer(vis_t* vis);

void handleSDLInput(int* eVar);

void showScreen(vis_t* vis);

void renderTestColor(vis_t* vis);

void drawTestBox(vis_t* vis);

void drawTestLine(vis_t* vis);

void drawTestPoint(vis_t* vis);
