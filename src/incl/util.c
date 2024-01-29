#include "util.h"

// Variables

const char* invalExt = "Extension not supported. Exiting...\n";

const int EC_invalExt = 0xBADF11E;

const char* validExts[] = {
    "mhs",
    "bmp"
};

const int valExtLen = sizeof(validExts) / sizeof(validExts[0]);

// Functions

void spewInfo(FILE* stream, char* extension){
    int extNum = stringWithinArray(extension, validExts, valExtLen);
    if(extNum == -1){
        errorOut(invalExt, EC_invalExt);
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
