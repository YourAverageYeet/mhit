/**
 * @file main.c
 * @author Mike Hensley (mike2google.tamu.edu)
 * @brief Main source file for MHIT
 * @version 0.1
 * @date 2024-01-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "incl/universal.h"
#include "incl/mhps.h"

int main(int argc, char* argv[]){
    FILE* spriteFile = fopen(argv[1], "rb");
    pSpr_t* spriteObject = genSpriteObj(spriteFile);
    displaySpriteData(spriteObject);
    int palNum = ((spriteObject->info->palInfo & 0xF0) >> 4) + 1;
    printf("\nDisplaying all %d sprite colorations...\n\n", palNum);
    for(int i = 0; i < palNum; i++){
        spriteToConsole(spriteObject, i);
        moveCursor(0, spriteObject->sprHeight);
        moveCursor(2, (spriteObject->sprWidth * 2));
    }
    cursorLinesDown(spriteObject->sprHeight + 1);
    destroySpriteObj(spriteObject);
    return 0;
}
