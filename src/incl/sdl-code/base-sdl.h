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
#include <time.h>

#include "gsv-icon.h"
#include "../universal.h"

#define SCR_W 640   ///< Default SDL window width
#define SCR_H 480   ///< Default SDL window height

#define BACK_R 0x23  ///< Deafult red channel value for backgound
#define BACK_G 0x26  ///< Deafult green channel value for backgound
#define BACK_B 0x27  ///< Deafult blue channel value for backgound

#define GIMP_RED    0x000000FF  ///< Red channel mask for GIMP-exported images
#define GIMP_GREEN  0x0000FF00  ///< Green channel mask for GIMP-exported images
#define GIMP_BLUE   0x00FF0000  ///< Blue channel mask for GIMP-exported images
#define GIMP_ALPHA  0xFF000000  ///< Alpha channel mask for GIMP-exported images

/**
 * @brief A small struct capable of representing a fully-functional window.
 * 
 */
typedef struct vis_s {
    SDL_Renderer*   rend;   ///< The renderer for this specific window
    SDL_Window*     wind;   ///< The actual window itself
} vis_t;

/**
 * @brief A struct that holds an X/Y coordinate pair used to position objects
 * within a visualizer.
 * 
 */
typedef struct objPos_s {
    int x_pos;
    int y_pos;
} objPos_t;

// BASE SDL FUNCTIONS

/**
 * @brief Creates an SDL renderer and window pair. Said pairs are termed
 * visualizers in this documentation.
 * 
 * @return vis_t* Pointer to the new visualizer
 */
vis_t* generateSDLVisualizer(void);

/**
 * @brief Small function that handles the destruction of a visualizer.
 * 
 * @param vis The visualizer to destroy.
 */
void destroySDLVisualizer(vis_t* vis);

/**
 * @brief As titled, this function handles inputs from SDL.
 * 
 * @param eVar the variable to track for an "exit" condition.
 */
void handleSDLInput(int* eVar);

/**
 * @brief Small wrapper function to `SDL_RenderPresent`.
 * 
 * @param vis The visualizer to update.
 */
void showScreen(vis_t* vis);

/**
 * @brief Plots a colored pixel at a given location.
 * 
 * @param vis The visualizer to use
 * @param pos The position of the pixel
 * @param r Red channel value
 * @param g Green channel value
 * @param b Blue channel value
 */
void placePixel(vis_t* vis, objPos_t pos, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Draws a filled box at a given location.
 * 
 * @param vis The visualizer to use.
 * @param pos The position of the box's upper-left corner
 * @param w The box's width
 * @param h The box's height
 * @param r Red chanel value
 * @param g Green chanel value
 * @param b Blue chanel value
 */
void drawBlock(vis_t* vis, objPos_t pos, uint16_t w, uint16_t h, uint8_t r,
                uint8_t g, uint8_t b);

// VIS_TEST SPECIFIC FUNCTIONS

/**
 * @brief Clears the screen to a bright, salmon-like color.
 * 
 * @param vis The visualizer to clear.
 */
void renderTestColor(vis_t* vis);

/**
 * @brief Draws a randomly-colored, ramdomly-sized box within a visualizer.
 * 
 * @param vis The visualizer to draw the box to.
 */
void drawTestBox(vis_t* vis);

/**
 * @brief Draws a randomly-colored line between two random points within a
 * visualizer.
 * 
 * @param vis The visualizer to draw the line to.
 */
void drawTestLine(vis_t* vis);

/**
 * @brief Draws a randomly-colored pixel to a random spot within a visualizer.
 * 
 * @param vis The visualizer to draw the pixel to.
 */
void drawTestPoint(vis_t* vis);

#endif // BASE_SDL_H_INCLUDED
