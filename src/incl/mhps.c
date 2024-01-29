#include "mhps.h"
#include "universal.h"
#include "ansi.h"
#include "bmp.h"

// Variables

const uint8_t mhps_magic[4] = {0x6D, 0x68, 0x70, 0x73};

const char consolePixel = ' ';

const char* mhpsMagic = "File does not contain proper magic number. Exiting...\n";

const int EC_mhpsMagic = 0x5BADF11E; // S(prite) Bad File

// MHPS Functions

void checkFileSig_mhps(FILE* inputFile){
    fseek(inputFile, 0, SEEK_SET);
    uint8_t magic_buff[4] = {};
    fread(&magic_buff, 1, 4, inputFile);
    if(memcmp(magic_buff, mhps_magic, 4)){
        errorOut(mhpsMagic, EC_mhpsMagic);
    }
}

sprInfo_t* getSpriteInfo(FILE* inputFile){
    checkFileSig_mhps(inputFile);
    sprInfo_t* newInfo = malloc(sizeof(sprInfo_t));
    uint8_t infoVals[4] = {0x00, 0x00, 0x00, 0x00};
    fseek(inputFile, 4, SEEK_SET);
    fread(infoVals, 1, 4, inputFile);
    newInfo->version = infoVals[0];
    newInfo->palCount = ((infoVals[1] & 0xF0) >> 4) + 1;
    newInfo->palSize = (infoVals[1] & 0x0F) + 1;
    newInfo->sprWidth = (uint64_t)1 << infoVals[2];
    newInfo->sprHeight = (uint64_t)1 << infoVals[3];
    return newInfo;
}

pSpr_t* genSpriteObj(FILE* inputFile){
    checkFileSig_mhps(inputFile);
    pSpr_t* newSprite = malloc(sizeof(pSpr_t));
    if(newSprite == NULL){
        errorOut(noMem, EC_noMem);
    } else {
        newSprite->info = getSpriteInfo(inputFile);
        fseek(inputFile, 8, SEEK_SET);
        uint64_t sprSize = newSprite->info->sprWidth * newSprite->info->sprHeight;
        uint16_t palSize = newSprite->info->palCount * newSprite->info->palSize;
        uint8_t* sprDataPtr = malloc(sprSize);
        uint8_t* palDataPtr = malloc(palSize * 3);
        fread(sprDataPtr, 1, sprSize, inputFile);
        fread(palDataPtr, 1, (palSize * 3), inputFile);
        newSprite->sprData = sprDataPtr;
        newSprite->palData = palDataPtr;
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
    printf("Sprite Version:\t%" PRId8, spriteObj->info->version);
    printf("\n\nSprite Bounds:\t%" PRId64 " by %" PRId64 " pixels",\
                spriteObj->info->sprWidth, spriteObj->info->sprHeight);
    puts("\nPixel Color Indices:");
    int spriteSize = spriteObj->info->sprWidth * spriteObj->info->sprHeight;
    for(int i = 0; i < spriteSize; i++){
        if(!(i % spriteObj->info->sprWidth)){
            puts("");
        }
        uint8_t colorIndex = spriteObj->sprData[i];
        if(colorIndex == 0xFF){
            printf("%c ", ' ');
        } else {
            printf("%X ", colorIndex);
        }
    }
    printf("\n\nThis sprite has %d palettes at %d colors per palette.\n",\
                spriteObj->info->palCount, spriteObj->info->palSize);
    puts("\nPalette Data:");
    int totalPalBytes = 3 * spriteObj->info->palCount * spriteObj->info->palSize;
    for(int i = 0; i < totalPalBytes; i++){
        if(!(i % 12)){
            puts("");
        }
        if(!((i + 1) % 3)){
            printf("0x%02X, ", spriteObj->palData[i]);
        } else {
            printf("0x%02X ", spriteObj->palData[i]);
        }
    }
    puts("");
}

void spriteToConsole(pSpr_t* spriteObj, int paletteNum){
    uint16_t palOffset = 3 * spriteObj->info->palSize * paletteNum;
    for(int r = 0; r < spriteObj->info->sprHeight; r++){
        for(int c = 0; c < spriteObj->info->sprWidth; c++){
            uint64_t pixelOffset = (r * spriteObj->info->sprWidth) + c;
            uint8_t pixelColor = spriteObj->sprData[pixelOffset];
            if(pixelColor == 0xFF){
                ansiTextReset();
                printf("%2c", consolePixel);
            } else {
                uint16_t colorOffset = palOffset + (3 * pixelColor);
                setTextColorTrue(spriteObj->palData[colorOffset],
                                 spriteObj->palData[colorOffset + 1],
                                 spriteObj->palData[colorOffset + 2]);
                printf("%2c", consolePixel);
            }
            ansiTextReset();
        }
        moveCursor(1, 1);
        moveCursor(3, (spriteObj->info->sprWidth * 2));
    }
}
