#ifndef MHPS_H_INCLUDED
#define MHPS_H_INCLUDED

#include "universal.h"

extern const uint8_t mhps_magic[4];

extern const char consolePixel;

typedef struct sprInfo_t {
    uint8_t version;
    uint8_t palInfo;
} sprInfo_t;

typedef struct pSpr_t {
    sprInfo_t* info;
    uint64_t sprWidth;
    uint64_t sprHeight;
    uint8_t* sprData;
    uint8_t* palData;
} pSpr_t;

void checkFileSig_mhps(FILE* inputFile);

sprInfo_t* getSpriteInfo(FILE* inputFile);

pSpr_t* genSpriteObj(FILE* inputFile);

void destroySpriteObj(pSpr_t* spriteObj);

void displaySpriteData(pSpr_t* spriteObj);

void spriteToConsole(pSpr_t* spriteObj, int paletteNum);

#endif //MHPS_H_INCLUDED