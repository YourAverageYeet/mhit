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

const int EC_tooManyColors = 0x22C01025; // Too (Many) Colors

const char* tooManyPals = "Supplied palettes image has to many rows. \
Exiting...";

const int EC_tooManyPals = 0x22220BAD; // Too Bad

const char* invalBMPMode = "Invalid mode supplied to spriteToBMPs. \
Exiting...\n";

const int EC_invalBMPMode = 0xBAD30DE; // Bad Mode (M rotated into 3)

// MHPS Functions

void checkFileSig_mhps(FILE* inputFile){
    fseek(inputFile, 0, SEEK_SET);
    uint8_t magic_buff[4] = {0, 0, 0, 0};
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
    newInfo->sprWidth = 8 * (infoVals[2] + 1);
    newInfo->sprHeight = 8 * (infoVals[3] + 1);
    return newInfo;
}

pSpr_t* genSpriteObj(FILE* inputFile){
    checkFileSig_mhps(inputFile);
    pSpr_t* newSprite = malloc(sizeof(pSpr_t));
    if(newSprite == NULL){
        errorOut(noMem, EC_noMem);
    }
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
    for(uint64_t r = 0; r < spriteObj->info->sprHeight; r++){
        uint8_t* rowBuff = malloc(spriteObj->info->sprWidth);
        for(uint64_t c = 0; c < spriteObj->info->sprWidth; c++){
            uint64_t pixelOffset = (r * spriteObj->info->sprWidth) + c;
            uint8_t pixelColor = spriteObj->sprData[pixelOffset];
            rowBuff[c] = pixelColor;
        }
        if(checkArrayFull_byte(rowBuff, 0xFF, spriteObj->info->sprWidth)){
            continue;
        } else {
            for(uint64_t i = 0; i < spriteObj->info->sprWidth; i++){
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
    puts("Sprite Info allocated");
    sprObj->info->version = 0x00;
    double sVal = (float)skeleton->deviceHeader->imageHeight / (float)8;
    printf("Calculated %lf from %d / 8\n", sVal, \
        skeleton->deviceHeader->imageHeight);
    sprObj->info->sprHeight = (uint8_t)ceil(sVal) * 8;
    sVal = (float)skeleton->deviceHeader->imageWidth / (float)8;
    printf("Calculated %lf from %d / 8\n", sVal, \
        skeleton->deviceHeader->imageWidth);
    sprObj->info->sprWidth = (uint8_t)ceil(sVal) * 8;
    if(palettes->deviceHeader->imageWidth > 16){
        errorOut(tooManyColors, EC_tooManyColors);
    }
    sprObj->info->palSize = palettes->deviceHeader->imageWidth;
    if(palettes->deviceHeader->imageHeight > 16){
        errorOut(tooManyPals, EC_tooManyPals);
    }
    sprObj->info->palCount = palettes->deviceHeader->imageHeight;
    puts("Sprite information set");
    sprObj->sprData = malloc(sprObj->info->sprWidth * sprObj->info->sprHeight\
                        * sizeof(uint8_t));
    if(sprObj->sprData == NULL){
        errorOut(noMem, EC_noMem);
    }
    puts("Sprite data allocated");
    uint64_t sprArea = (uint64_t)sprObj->info->sprWidth *\
        (uint64_t)sprObj->info->sprHeight;
    for(uint64_t i = 0; i < sprArea; i++){
        sprObj->sprData[i] = 0xFF;
    }
    double diff_x = (double)sprObj->info->sprWidth -\
        (double)skeleton->deviceHeader->imageWidth;
    uint8_t off_x = (uint8_t)floor(diff_x / 2);
    double diff_y = (double)sprObj->info->sprHeight -\
        (double)skeleton->deviceHeader->imageHeight;
    uint8_t off_y = (uint8_t)ceil(diff_y / 2);
    printf("Offsets into sprite area:\n\tRows: %" PRIu8 "\tColumns: %" PRIu8\
        "\n", off_y, off_x);
    for(int y = off_y; y < skeleton->deviceHeader->imageHeight + off_y; y++){
        int baseSprOffset = y * sprObj->info->sprWidth;
        int baseBmpOffset = (y - off_y) * skeleton->deviceHeader->imageWidth;
        for(int x = off_x; x < skeleton->deviceHeader->imageWidth + off_x; x++){
            uint32_t color = skeleton->pixelArray[baseBmpOffset + x - off_x];
            if(((color & 0xFF000000) == 0xFF000000)){
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
    puts("Sprite data set");
    int tmp = sprObj->info->palCount * sprObj->info->palSize * 3;
    printf("Will allocate %d bytes...", tmp);
    sprObj->palData = malloc(tmp);
    if(sprObj->palData == NULL){
        errorOut(noMem, EC_noMem);
    }
    puts("Palette data allocated");
    for(int i = 0; i < (sprObj->info->palCount * sprObj->info->palSize); i++){
        int palOff = i * 3;
        uint32_t color = palettes->pixelArray[i];
        sprObj->palData[palOff] = (uint8_t)((color & 0xFF0000) >> 16);
        sprObj->palData[palOff + 1] = (uint8_t)((color & 0xFF00) >> 8);
        sprObj->palData[palOff + 2] = (uint8_t)(color & 0xFF);
    }
    puts("Palette data set");
    destroyRawBMP(skeleton);
    destroyRawBMP(palettes);
    return sprObj;
}

void spriteToFile(pSpr_t* sprite, char* name){
    char lastFour[5] = {0, 0, 0, 0, 0};
    int strEnd = strlen(name);
    lastFour[3] = name[strEnd-1];
    lastFour[2] = name[strEnd-2];
    lastFour[1] = name[strEnd-3];
    lastFour[0] = name[strEnd-4];
    if(strcmp(lastFour, ".mhs") != 0){
        name[strEnd] = '.';
        name[strEnd + 1] = 'm';
        name[strEnd + 2] = 'h';
        name[strEnd + 3] = 's';
        name[strEnd + 4] = '\000';
    }
    FILE* sprFile = fopen(name, "wb");
    fwrite(mhps_magic, 1, 4, sprFile);
    fwrite(&sprite->info->version, 1, 1, sprFile);
    uint8_t palInfo = (sprite->info->palCount - 1) << 4;
    palInfo += (sprite->info->palSize - 1);
    fwrite(&palInfo, 1, 1, sprFile);
    uint8_t size = (sprite->info->sprWidth / 8) - 1;
    fwrite(&size, 1, 1, sprFile);
    size = (sprite->info->sprHeight / 8) - 1;
    fwrite(&size, 1, 1, sprFile);
    uint64_t writeBytes = sprite->info->sprWidth * sprite->info->sprHeight;
    fwrite(sprite->sprData, 1, writeBytes, sprFile);
    writeBytes = sprite->info->palCount * sprite->info->palSize * 3;
    fwrite(sprite->palData, 1, writeBytes, sprFile);
    printf("\nData written to %s\n", name);
    fclose(sprFile);
}

void spriteToBMPs(pSpr_t* sprite, int mode){
    if(mode > 1){
        errorOut(invalBMPMode, EC_invalBMPMode);
    } else if(mode) {
        uint64_t iW = sprite->info->sprWidth;
        uint64_t iH = sprite->info->sprHeight;
        uint32_t imgSize = iW * iH;
        uint32_t fSizeB = imgSize * 4;
        char outName[14] = "paletteNN.bmp";
        for(uint8_t p = 0; p < sprite->info->palCount; p++){
            bmpRawFile_t* paletted = genEmptyRawBMP();
            paletted->deviceHeader->imageWidth = (int32_t)iW;
            paletted->deviceHeader->imageHeight = -(int32_t)iH;
            paletted->deviceHeader->imageSize = imgSize * 4;
            paletted->fileHeader->fileSize = paletted->fileHeader->offToPixels;
            paletted->fileHeader->fileSize += fSizeB;
            paletted->pixelArray = malloc(imgSize * sizeof(uint32_t));
            memset(paletted->pixelArray, 0, (imgSize * 4));
            printf("Allocated %u bytes\t(%u pixels)\n", (imgSize * 4),
                    imgSize);
            uint16_t palOff = sprite->info->palSize * p * 3;
            for(uint32_t i = 0; i < imgSize; i++){
                uint8_t read = sprite->sprData[i];
                if(read != 0xFF){
                    read &= 0x0F;
                    uint16_t colorOff = palOff + (read * 3);
                    argbColor_t color = {0, 0, 0, 0};
                    color.alpha = 0xFF;
                    color.red = sprite->palData[colorOff];
                    color.green = sprite->palData[colorOff + 1];
                    color.blue = sprite->palData[colorOff + 2];
                    uint32_t write = colorToLong(color);
                    paletted->pixelArray[i] = write;
                } else {
                    paletted->pixelArray[i] = 0x00000000;
                }
            }
            sprintf(outName, "palette%02hhu.bmp", (p + 1));
            saveBMPFile(paletted, outName);
            destroyRawBMP(paletted);
        }
    } else {
        bmpRawFile_t* skelRaw = genEmptyRawBMP();
        skelRaw->deviceHeader->imageWidth = (int32_t)sprite->info->sprWidth;
        skelRaw->deviceHeader->imageHeight = -(int32_t)sprite->info->sprHeight;
        uint32_t imgSize = sprite->info->sprWidth * sprite->info->sprHeight;
        skelRaw->deviceHeader->imageSize = imgSize * 4;
        uint32_t fSize = skelRaw->fileHeader->offToPixels + (imgSize * 4);
        skelRaw->fileHeader->fileSize = fSize;
        skelRaw->pixelArray = malloc(imgSize * sizeof(uint32_t));
        memset(skelRaw->pixelArray, 0, (imgSize * 4));
        printf("Allocated %u bytes\t(%u pixels)\n", (imgSize * 4), imgSize);
        for(size_t i = 0; i < imgSize; i++){
            uint8_t read = sprite->sprData[i];
            uint32_t written = 0;
            if(read != 0xFF){
                for(int s = 0; s < 8; s++){
                    written |= (read << (4 * s));
                }
                written |= 0xFF000000;
                skelRaw->pixelArray[i] = written;
            } else {
                skelRaw->pixelArray[i] = written;
            }
        }
        saveBMPFile(skelRaw, "skeleton.bmp");
        destroyRawBMP(skelRaw);
        bmpRawFile_t* palsRaw = genEmptyRawBMP();
        palsRaw->deviceHeader->imageWidth = (int32_t)sprite->info->palSize;
        palsRaw->deviceHeader->imageHeight = -(int32_t)sprite->info->palCount;
        imgSize = sprite->info->palSize * sprite->info->palCount;
        palsRaw->deviceHeader->imageSize = imgSize * 4;
        fSize = palsRaw->fileHeader->offToPixels + (imgSize * 4);
        palsRaw->fileHeader->fileSize = fSize;
        palsRaw->pixelArray = malloc(imgSize * sizeof(uint32_t));
        memset(palsRaw->pixelArray, 0, (imgSize * 4));
        printf("Allocated %u bytes\t(%u pixels)\n", (imgSize * 4), imgSize);
        for(size_t i = 0; i < imgSize; i++){
            size_t ci = i * 3;
            argbColor_t wColor = {0, 0, 0, 0};
            wColor.alpha = 0xFF;
            wColor.red = sprite->palData[ci];
            wColor.green = sprite->palData[ci + 1];
            wColor.blue = sprite->palData[ci + 2];
            uint32_t written = colorToLong(wColor);
            palsRaw->pixelArray[i] = written;
        }
        saveBMPFile(palsRaw, "palettes.bmp");
        destroyRawBMP(palsRaw);
    }
    puts("Sprite Converted!");
}
