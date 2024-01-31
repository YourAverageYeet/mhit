#include "util.h"

// Variables

const char* invalInfoExt = "Extension not supported. Exiting...\n";

const char* invalDisplayExt = "Passed file is not a sprite file. Exiting...\n";

const int EC_invalExt = 0xBADF11E;

const char* validInfoExts[] = {
    "mhs",
    "bmp"
};

const char* validDisplayExts[] = {
    "mhs"
};

const int valIExtLen = sizeof(validInfoExts) / sizeof(validInfoExts[0]);

const int valDExtLen = sizeof(validDisplayExts) / sizeof(validDisplayExts[0]);

// Functions

void spewInfo(FILE* stream, char* extension){
    int extNum = stringWithinArray(extension, validInfoExts, valIExtLen);
    if(extNum == -1){
        errorOut(invalInfoExt, EC_invalExt);
    }
    switch(extNum){
        case(0):
            pSpr_t* spr = genSpriteObj(stream);
            displaySpriteData(spr);
            destroySpriteObj(spr);
            break;
        case(1):
            bmpRawFile_t* bmp = createRawBMP(stream);
            bmpDetailsOut(bmp);
            destroyRawBMP(bmp);
            break;
        default:
            errorOut(switchDef, EC_switchDef);
    }
}

void spriteDisplay(FILE* stream, char* extension){
    int extNum = stringWithinArray(extension, validDisplayExts, valDExtLen);
    if(extNum == -1){
        errorOut(invalDisplayExt, EC_invalExt);
    }
    switch(extNum){
        case(0):
            int palNum = 0;
            pSpr_t* spr = genSpriteObj(stream);
            printf("Please pick a number between 1 and %d\t-> ",\
                    spr->info->palCount);
            scanf("%d", &palNum);
            palNum--;
            spriteToConsole(spr, palNum);
            destroySpriteObj(spr);
            break;
        default:
            errorOut(switchDef, EC_switchDef);
            break;
    }
}
