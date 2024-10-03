#include "mhps-disp.h"

objPos_t* findCenterOffsets(pSpr_t* spr){
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

void drawSpriteSDL(vis_t* vis, pSpr_t* spr, int pal, int scale, objPos_t* coords){
    int drawx = coords->x_pos;
    int drawy = coords->y_pos;
    for(uint64_t r = 0; r < spr->info->sprHeight; r++){
        for(uint64_t c = 0; c < spr->info->sprWidth; c++){
            int pxoff = (r * spr->info->sprWidth) + c;
            uint8_t color_num = spr->sprData[pxoff];
            if(color_num == 0xFF){
                drawx += scale;
                continue;
            }
            int pal_base = pal * 3 * spr->info->palSize;
            uint8_t color_r = spr->palData[pal_base + 3 * color_num];
            uint8_t color_g = spr->palData[pal_base + 3 * color_num + 1];
            uint8_t color_b = spr->palData[pal_base + 3 * color_num + 2];
            objPos_t point = {drawx, drawy};
            if(scale == 1){
                placePixel(vis, point, color_r, color_g, color_b);
            } else {
                drawBlock(vis, point, scale, scale, color_r, color_g, color_b);
            }
            drawx += scale;
        }
        drawx = coords->x_pos;
        drawy += scale;
    }
}

void displayMHPS(vis_t* vis, char* sprPath){
    int exitVar = 0;
    unsigned char pal = 0;
    unsigned char scale = 1;
    SDL_Event event = {0};
    FILE* sprFile = fopen(sprPath, "rb");
    pSpr_t* mhs = genSpriteObj(sprFile);
    objPos_t* sPos = findCenterOffsets(mhs);
    int farX = SCR_W - mhs->info->sprWidth;
    int farY = SCR_H - mhs->info->sprHeight;
    char* sprName = strrchr(sprPath, '/');
    sprName = sprName ? sprName + 1 : sprPath; 
    char* nameStr = malloc(14 + strlen(sprName));
    char* posStr = malloc(30);
    char* palStr = malloc(25);
    char* scaleStr = malloc(18);
    sprintf(nameStr, "Sprite Name: %s", sprName);
    objPos_t nameStrPos = {2, 2};
    objPos_t posStrPos = {2, 10};
    objPos_t palStrPos = {2, 18};
    objPos_t scaleStrPos = {2, 26};
    while(!exitVar){
        SDL_SetRenderDrawColor(vis->rend, 0x45, 0x45, 0x45, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(vis->rend);
        drawSpriteSDL(vis, mhs, pal, scale, sPos);
        sprintf(posStr, "Sprite Position: ( %d , %d )", sPos->x_pos,\
            sPos->y_pos);
        sprintf(palStr, "Palette Number: %d of %d", (pal + 1),\
            mhs->info->palCount);
        sprintf(scaleStr, "Sprite Scale: %dx", scale);
        minifString(vis, nameStrPos, nameStr);
        minifString(vis, posStrPos, posStr);
        minifString(vis, palStrPos, palStr);
        minifString(vis, scaleStrPos, scaleStr);
        showScreen(vis);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case(SDLK_ESCAPE):
                        exitVar++;
                        break;
                    case(SDLK_LEFT):
                        if(sPos->x_pos > 0){
                            sPos->x_pos -= scale;
                        }
                        break;
                    case(SDLK_RIGHT):
                        if(sPos->x_pos < farX){
                            sPos->x_pos += scale;
                        }
                        break;
                    case(SDLK_UP):
                        if(sPos->y_pos > 0){
                            sPos->y_pos -= scale;
                        }
                        break;
                    case(SDLK_DOWN):
                        if(sPos->y_pos < farY){
                            sPos->y_pos += scale;
                        }
                        break;
                    case(SDLK_LEFTBRACKET):
                        if(pal != 0){
                            pal--;
                        }
                        break;
                    case(SDLK_RIGHTBRACKET):
                        pal++;
                        pal %= mhs->info->palCount;
                        break;
                    case(SDLK_TAB):
                        sPos = findCenterOffsets(mhs);
                        pal = 0;
                        scale = 1;
                        break;
                    case(SDLK_PERIOD):
                        if(scale < 64){
                            scale <<= 1;
                        }
                        break;
                    case(SDLK_COMMA):
                        if(scale > 1){
                            scale >>= 1;
                        }
                        break;
                }
            } else if(event.type == SDL_QUIT){
                exitVar++;
            }
        }
        SDL_Delay(16);
    }
    destroySpriteObj(mhs);
    fclose(sprFile);
    free(sPos);
}
