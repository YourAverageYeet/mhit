/**
 * @file disp-base.h
 * @author Mike Hensley (mike2@tamu.edu)
 * @brief 
 * @version 0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MHPS_DISP_INCLUDED
#define MHPS_DISP_INCLUDED

#include "../base-sdl.h"
#include "../../mhps.h"
#include "../minif.h"

/**
 * @brief Finds the offests with which to display a sprite to center it within a
 * visualizer.
 * 
 * @param spr The sprite to find the offsets for
 * @return objPos_t* 
 */
objPos_t* findCenterOffsets(pSpr_t* spr);

/**
 * @brief Draws a sprite at the given position within a given visualizer.
 * 
 * @param vis The visualizer to draw the sprite within.
 * @param spr The sprite to draw.
 * @param pal The palette to use when drawing the sprite.
 * @param coords The coordinates at which to place the sprite's top-left corner.
 */
void drawSpriteSDL(vis_t* vis, pSpr_t* spr, int pal, int scale, objPos_t* coords);

void displayMHPS(vis_t* vis, char* sprPath);

#endif // MHPS_DISP_INCLUDED
