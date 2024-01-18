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

/**
 * @brief The `main` function of MHIT
 * 
 * @param argc The amount of arguments passed
 * @param argv The vectors to those arguments
 * @return int The program return value under normal circumstances
 */
int main(int argc, char* argv[]){
    FILE* spriteFile = fopen(argv[1], "rb");
    pSpr_t* spriteObject = genSpriteObj(spriteFile);
    displaySpriteData(spriteObject);;
    printf("\nDisplaying all %d sprite colorations...\n\n",\
                spriteObject->info->palCount);
    for(int i = 0; i < spriteObject->info->palCount; i++){
        spriteToConsole(spriteObject, i);
        moveCursor(0, spriteObject->info->sprHeight);
        moveCursor(2, ((spriteObject->info->sprWidth * 2) + 2));
    }
    cursorLinesDown(spriteObject->info->sprHeight + 1);
    destroySpriteObj(spriteObject);
    return 0;
}
