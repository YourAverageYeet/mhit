#include "mhps-disp.h"

objPos_t* findSprOffsets(pSpr_t* spr){
    objPos_t* coords = malloc(sizeof(objPos_t));
    double tmpVal = SCR_W - spr->info->sprWidth;
    tmpVal /= 2;
    tmpVal = floor(tmpVal);
    coords->x_pos = (int)tmpVal;
    tmpVal = SCR_H - spr->info->sprHeight;
    tmpVal /= 2;
    tmpVal = floor(tmpVal);
    coords->y_pos = (int)tmpVal;
    return coords;
}

void drawSpriteSDL(vis_t* vis, pSpr_t* spr, int pal, objPos_t* coords){
    SDL_SetRenderDrawColor(vis->rend, 0x45, 0x45, 0x45, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(vis->rend);
    int drawx = coords->x_pos;
    int drawy = coords->y_pos;
    for(int r = 0; r < spr->info->sprHeight; r++){
        for(int c = 0; c < spr->info->sprWidth; c++){
            int pxoff = (r * spr->info->sprWidth) + c;
            uint8_t color_num = spr->sprData[pxoff];
            if(color_num == 0xFF){
                drawx++;
                continue;
            }
            int pal_base = pal * 3 * spr->info->palSize;
            uint8_t color_r = spr->palData[pal_base + 3 * color_num];
            uint8_t color_g = spr->palData[pal_base + 3 * color_num + 1];
            uint8_t color_b = spr->palData[pal_base + 3 * color_num + 2];
            objPos_t point = {drawx, drawy};
            placePixel(vis, point, color_r, color_g, color_b);
            drawx++;
        }
        drawx = coords->x_pos;
        drawy++;
    }
}
