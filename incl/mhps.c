#include "mhps.h"
#include "universal.h"
#include "ansi.h"

const uint8_t mhps_magic[4] = {0x6D, 0x68, 0x70, 0x73};

const char consolePixel = ' ';

void checkFileSig_mhps(FILE* inputFile){
    fseek(inputFile, 0, SEEK_SET);
    uint8_t magic_buff[4] = {};
    fread(&magic_buff, 1, 4, inputFile);
    if(memcmp(magic_buff, mhps_magic, 4)){
        fprintf(stderr, "File does not contain proper magic number. Exiting...\n");
        exit(0xBADF11E); // BADFILE
    }
}

sprInfo_t* getSpriteInfo(FILE* inputFile){
    checkFileSig_mhps(inputFile);
    sprInfo_t* newInfo = malloc(sizeof(sprInfo_t));
    uint8_t infoVals[2] = {0x00, 0x00};
    fseek(inputFile, 4, SEEK_SET);
    fread(infoVals, 1, 2, inputFile);
    newInfo->version = infoVals[0];
    newInfo->palInfo = infoVals[1];
    return newInfo;
}

pSpr_t* genSpriteObj(FILE* inputFile){
    checkFileSig_mhps(inputFile);
    pSpr_t* newSprite = malloc(sizeof(pSpr_t));
    if(newSprite == NULL){
        fprintf(stderr, "genSpriteObj: Not enough memory error. Exiting...\n");
        exit(0xBAD0B3EC7); // BADOBJECT
    } else {
        newSprite->info = getSpriteInfo(inputFile);
        fseek(inputFile, 6, SEEK_SET);
        uint8_t byteBuff[2] = {};
        fread(&byteBuff, 1, 2, inputFile);
        newSprite->sprWidth = (uint64_t)1 << byteBuff[0];
        newSprite->sprHeight = (uint64_t)1 << byteBuff[1];
        int sprSize = newSprite->sprWidth * newSprite->sprHeight;
        uint8_t* sprPixels = malloc(sizeof(uint8_t) * sprSize);
        fseek(inputFile, 8, SEEK_SET);
        fread(sprPixels, 1, sprSize, inputFile);
        uint8_t palByte = newSprite->info->palInfo;
        int palSize = (((palByte & 0xF0) >> 4) + 1) * ((palByte & 0x0F) + 1) * 3;
        uint8_t* palColors = malloc(sizeof(uint8_t) * palSize);
        fseek(inputFile, (8 + sprSize), SEEK_SET);
        fread(palColors, 1, palSize, inputFile);
        newSprite->sprData = sprPixels;
        newSprite->palData = palColors;
        return newSprite;
    }
}

void destroySpriteObj(pSpr_t* spriteObj){
    free(spriteObj->info);
    free(spriteObj->sprData);
    free(spriteObj->palData);
    free(spriteObj);
}

void displaySpriteData(pSpr_t* spriteObj){
    printf("\nSprite Version:\t%" PRId8, spriteObj->info->version);
    printf("\n\nSprite Bounds:\t%" PRId64 " by %" PRId64 " pixels",\
                spriteObj->sprWidth, spriteObj->sprHeight);
    puts("\nPixel Color Indices:");
    int spriteSize = spriteObj->sprWidth * spriteObj->sprHeight;
    for(int i = 0; i < spriteSize; i++){
        if(!(i % spriteObj->sprWidth)){
            puts("");
        }
        uint8_t colorIndex = spriteObj->sprData[i];
        if(colorIndex == 0xFF){
            printf("%c ", ' ');
        } else {
            printf("%X ", colorIndex);
        }
    }
    int palCount = ((spriteObj->info->palInfo & 0xF0) >> 4) + 1;
    int colCount = (spriteObj->info->palInfo & 0x0F) + 1;
    printf("\n\nThis sprite has %d palettes at %d colors per palette.\n", palCount,\
                colCount);
    puts("\nPalette Data:");
    int totalPalBytes = 3 * (((spriteObj->info->palInfo & 0xF0) >> 4) + 1) *\
                            ((spriteObj->info->palInfo & 0x0F) + 1);
    for(int i = 0; i < totalPalBytes; i++){
        if(!(i % 12)){
            puts("");
        }
        if(!((i + 1) % 3)){
            printf("0x%2X, ", spriteObj->palData[i]);
        } else {
            printf("0x%2X ", spriteObj->palData[i]);
        }
    }
}

void spriteToConsole(pSpr_t* spriteObj, int paletteNum){
    uint8_t colPerPal = (spriteObj->info->palInfo & 0x0F) + 1;
    uint16_t palOffset = 3 * colPerPal * paletteNum;
    for(int r = 0; r < spriteObj->sprHeight; r++){
        for(int c = 0; c < spriteObj->sprWidth; c++){
            uint64_t pixelOffset = (r * spriteObj->sprWidth) + c;
            uint8_t pixelColor = spriteObj->sprData[pixelOffset];
            if(pixelColor == 0xFF){
                resetTextColor();
                printf("%2c", consolePixel);
            } else {
                uint16_t colorOffset = palOffset + (3 * pixelColor);
                setTextColorTrue(spriteObj->palData[colorOffset],
                                 spriteObj->palData[colorOffset + 1],
                                 spriteObj->palData[colorOffset + 2]);
                printf("%2c", consolePixel);
            }
            resetTextColor();
        }
        moveCursor(1, 1);
        moveCursor(3, (spriteObj->sprWidth * 2));
    }
}
