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
#ifndef BASE_SDL_H_INCLUDED
#define BASE_SDL_H_INCLUDED

#include <SDL2/SDL.h>

#define SCR_W 640   ///< Default SDL window width
#define SCR_H 480   ///< Default SDL window height

#define BACK_R 0x23  ///< Deafult red channel value for backgound
#define BACK_G 0x26  ///< Deafult green channel value for backgound
#define BACK_B 0x27  ///< Deafult blue channel value for backgound

/**
 * @brief A small struct capable of representing a fully-functional window.
 * 
 */
typedef struct vis_s {
    SDL_Renderer*   rend;   ///< The renderer for this specific window
    SDL_Window*     wind;   ///< The actual window itself
} vis_t;

typedef struct objPos_s {
    int x_pos;
    int y_pos;
} objPos_t;

// BASE SDL FUNCTIONS


vis_t* generateSDLVisualizer(void);


void destroySDLVisualizer(vis_t* vis);


void handleSDLInput(int* eVar);


void showScreen(vis_t* vis);


void placePixel(vis_t* vis, uint16_t x, uint16_t y, uint8_t r, uint8_t g,
                uint8_t b);


void drawBlock(vis_t* vis, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                uint8_t r, uint8_t g, uint8_t b);

// VIS_TEST SPECIFIC FUNCTIONS

/**
 * @brief Clears the screen to a dark, salmon-like color.
 * 
 * @param vis The visualizer to clear.
 */
void renderTestColor(vis_t* vis);

/**
 * @brief Draws a randomly-colored, ramdomply-sized box within a window.
 * 
 * @param vis The visualizer to draw the box to.
 */
void drawTestBox(vis_t* vis);

/**
 * @brief Draws a randomly-colored line between two random poins within a
 * window.
 * 
 * @param vis The visualizer to draw the line to.
 */
void drawTestLine(vis_t* vis);

/**
 * @brief Draws a randomly-colored pixel to a random spot within a window.
 * 
 * @param vis The visualizer to draw the pixel to.
 */
void drawTestPoint(vis_t* vis);

#endif // BASE_SDL_H_INCLUDED
