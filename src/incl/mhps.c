#include "mhps.h"
#include "universal.h"
#include "ansi.h"
#include "bmp.h"

// Variables

const uint8_t mhps_magic[4] = {0x6D, 0x68, 0x70, 0x73};

const char consolePixel = ' ';

const char* mhpsMagic = "File does not contain proper magic number. \
                            Exiting...\n";

const int EC_mhpsMagic = 0x5BADF11E; // S(prite) Bad File

const char* tooManyColors = "Supplied palettes image has more than 16 colors \
                            per row. Exiting...";

const int EC_tooManyColors = 0x22C01025; // 22 Colors

const char* tooManyPals = "Supplied palettes image has to many rows. \
                            Exiting...";

const int EC_tooManyPals = 0x22220BAD; // Too Bad

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
    printf("\nSprite Version:\t%" PRId8, spriteObj->info->version);
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
    for(int i = 0; i < spriteObj->info->palCount; i++){
        for(int j = 0; j < (3 * spriteObj->info->palSize); j++){
            int offset = (i * (3 * spriteObj->info->palSize)) + j;
            if(!((j + 1) % 3)){
                printf("0x%02X, ", spriteObj->palData[offset]);
            } else {
                printf("0x%02X ", spriteObj->palData[offset]);
            }
        }
        for(int j = 0; j < spriteObj->info->palSize; j++){
            int offset = (i * spriteObj->info->palSize * 3) + (j * 3);
            setTextColorTrue(spriteObj->palData[offset],
                              spriteObj->palData[offset + 1],
                              spriteObj->palData[offset + 2]);
            printf("%2c", consolePixel);
        }
        ansiTextReset();
        puts("");
    }
    puts("");
}

void spriteToConsole(pSpr_t* spriteObj, int paletteNum){
    uint16_t palOffset = 3 * spriteObj->info->palSize * paletteNum;
    uint8_t* rowBuff = malloc(spriteObj->info->sprWidth);
    for(int r = 0; r < spriteObj->info->sprHeight; r++){
        uint8_t* rowBuff = malloc(spriteObj->info->sprWidth);
        for(int c = 0; c < spriteObj->info->sprWidth; c++){
            uint64_t pixelOffset = (r * spriteObj->info->sprWidth) + c;
            uint8_t pixelColor = spriteObj->sprData[pixelOffset];
            rowBuff[c] = pixelColor;
        }
        if(checkArrayFull_byte(rowBuff, 0xFF, spriteObj->info->sprWidth)){
            continue;
        } else {
            for(int i = 0; i < spriteObj->info->sprWidth; i++){
                if(rowBuff[i] == 0xFF){
                    ansiTextReset();
                    printf("%2c", consolePixel);
                    continue;
                }
                uint16_t colorOffset = palOffset + (3 * rowBuff[i]);
                setTextColorTrue(spriteObj->palData[colorOffset],
                                    spriteObj->palData[colorOffset + 1],
                                    spriteObj->palData[colorOffset + 2]);
                printf("%2c", consolePixel);
            }
        }
        ansiTextReset();
        moveCursor(CUR_DOWN, 1);
        moveCursor(CUR_LEFT, (spriteObj->info->sprWidth * 2));
        free(rowBuff);
    }
}

// CONVERSION FUNCTIONS

pSpr_t* rawBMPsToSprite(bmpRawFile_t* skeleton, bmpRawFile_t* palettes){
    pSpr_t* sprObj = malloc(sizeof(pSpr_t));
    if(sprObj == NULL){
        errorOut(noMem, EC_noMem);
    }
    sprObj->info = malloc(sizeof(sprInfo_t));
    if(sprObj->info == NULL){
        errorOut(noMem, EC_noMem);
    }
    sprObj->info->version = 0x00;
    for(int i = 0; i < 256; i++){
        uint64_t sVal = (uint64_t)1 << i;
        if(((uint64_t)1 << i) >= skeleton->deviceHeader->imageWidth){
            sprObj->info->sprWidth = sVal;
            break;
        }
    }
    for(int i = 0; i < 256; i++){
        uint64_t sVal = (uint64_t)1 << i;
        if(sVal >= skeleton->deviceHeader->imageHeight){
            sprObj->info->sprHeight = sVal;
            break;
        }
    }
    if(palettes->deviceHeader->imageWidth > 16){
        errorOut(tooManyColors, EC_tooManyColors);
    }
    sprObj->info->palSize = palettes->deviceHeader->imageWidth;
    if(palettes->deviceHeader->imageHeight > 16){
        errorOut(tooManyPals, EC_tooManyPals);
    }
    sprObj->info->palCount = palettes->deviceHeader->imageHeight;
    sprObj->sprData = malloc(sprObj->info->sprWidth * sprObj->info->sprHeight\
                        * sizeof(uint8_t));
    if(sprObj->sprData == NULL){
        errorOut(noMem, EC_noMem);
    }
    for(int y = 0; y < skeleton->deviceHeader->imageHeight; y++){
        int baseSprOffset = y * sprObj->info->sprWidth;
        int baseBmpOffset = y * skeleton->deviceHeader->imageWidth;
        for(int x = 0; x < skeleton->deviceHeader->imageWidth; x++){
            uint32_t color = skeleton->pixelArray[baseBmpOffset + x];
            if((color & 0xFF000000) == 0xFF000000){
                uint8_t r = (uint8_t)((color & 0xFF0000) >> 16);
                uint8_t g = (uint8_t)((color & 0xFF00) >> 8);
                uint8_t b = (uint8_t)(color & 0xFF);
                if((r == g) && (g == b)){
                    sprObj->sprData[baseSprOffset + x] = (r & 0x0F);
                }
            } else {
                sprObj->sprData[baseSprOffset + x] = 0xFF;
            }
        }
    }
    sprObj->palData = malloc(sprObj->info->palCount * sprObj->info->palSize * 3\
                                * sizeof(uint8_t));
    if(sprObj->palData == NULL){
        errorOut(noMem, EC_noMem);
    }
    for(int i = 0; i < (sprObj->info->palCount * sprObj->info->palSize); i++){
        int palOff = i * 3;
        uint32_t color = palettes->pixelArray[i];
        sprObj->palData[palOff] = (uint8_t)((color & 0xFF0000) >> 16);
        sprObj->palData[palOff + 1] = (uint8_t)((color & 0xFF00) >> 8);
        sprObj->palData[palOff + 2] = (uint8_t)(color & 0xFF);
    }
    destroyRawBMP(skeleton);
    destroyRawBMP(palettes);
    return sprObj;
}

void spriteToFile(pSpr_t* sprite, char* name){
    int strEnd = strlen(name);
    name[strEnd] = '.';
    name[strEnd + 1] = 'm';
    name[strEnd + 2] = 'h';
    name[strEnd + 3] = 's';
    name[strEnd + 4] = '\000';
    FILE* sprFile = fopen(name, "wb");
    fwrite(mhps_magic, 1, 4, sprFile);
    fwrite(&sprite->info->version, 1, 1, sprFile);
    uint8_t palInfo = (sprite->info->palCount - 1) << 4;
    palInfo += (sprite->info->palSize - 1);
    fwrite(&palInfo, 1, 1, sprFile);
    int shifts = 0;
    int dim = sprite->info->sprWidth;
    while(dim != 1){
        dim >>= 1;
        shifts++;
    }
    fwrite(&shifts, 1, 1, sprFile);
    shifts = 0;
    dim = sprite->info->sprHeight;
    while(dim != 1){
        dim >>= 1;
        shifts++;
    }
    fwrite(&shifts, 1, 1, sprFile);
    uint64_t writeBytes = sprite->info->sprWidth * sprite->info->sprHeight;
    fwrite(sprite->sprData, 1, writeBytes, sprFile);
    writeBytes = sprite->info->palCount * sprite->info->palSize * 3;
    fwrite(sprite->palData, 1, writeBytes, sprFile);
    printf("\nData written to %s\n", name);
    fclose(sprFile);
}
