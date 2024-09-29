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

#ifndef DISP_BASE_INCLUDED
#define DISP_BASE_INCLUDED

#include "../base-sdl.h"
#include "../../mhps.h"

objPos_t* findSprOffsets(pSpr_t* spr);

void drawSpriteSDL(vis_t* vis, pSpr_t* spr, int pal, objPos_t* coords);

#endif // DISP_BASE_INCLUDED
