/**
 * @file mhps.h
 * @author Mike Hensley (mike2@google.tamu.edu)
 * @brief Functions, types, and variables for working with MHPS files
 * @version 0.1
 * @date 2024-01-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef MHPS_H_INCLUDED
#define MHPS_H_INCLUDED

#include "universal.h"

/**
 * @brief The magic number for MHPS files; should be a lowercase `mhps`.
 * 
 */
extern const uint8_t mhps_magic[4];

/**
 * @brief Just a fancy name for the space character; used in `spriteToConsole()`.
 * 
 */
extern const char consolePixel;

/**
 * @brief Contains information about the sprite.
 * 
 */
typedef struct sprInfo_t {
    uint8_t version;    ///< The version of the read sprite.
    uint8_t palCount;   ///< The number of palette for this sprite.
    uint8_t palSize;    ///< The amount of colors per palette.
    uint64_t sprWidth;  ///< The width of the sprite.
    uint64_t sprHeight; ///< The height of the sprite.
} sprInfo_t;

/**
 * @brief This struct contains the actual sprite data itself.
 * 
 */
typedef struct pSpr_t {
    sprInfo_t* info;    ///< A pointer to the information about this sprite.
    uint8_t* sprData;   ///< The actual pixelmap of the sprite.
    uint8_t* palData;   ///< The actual palette information of the sprite.
} pSpr_t;

/**
 * @brief Checks whether the supplied file is a proper MHPS file or not.
 * 
 * @param inputFile File buffer of the file to be checked.
 */
void checkFileSig_mhps(FILE* inputFile);

/**
 * @brief Get sprite information from file header.
 * 
 * @param inputFile File buffer of the file to be read.
 * @return sprInfo_t* The generated information struct.
 */
sprInfo_t* getSpriteInfo(FILE* inputFile);

/**
 * @brief Generate a sprite object to be used by other functions.
 * 
 * @param inputFile The source file to generate a sprite object from.
 * @return pSpr_t* The generated sprite object.
 */
pSpr_t* genSpriteObj(FILE* inputFile);

/**
 * @brief Destroys a sprite object anf frees its memory back to the system.
 * 
 * @param spriteObj The sprite object to be destroyed.
 */
void destroySpriteObj(pSpr_t* spriteObj);

/**
 * @brief Prints the sprite's information to the console, among other useful things.
 * 
 * @param spriteObj The sprite object to report to the user.
 */
void displaySpriteData(pSpr_t* spriteObj);

/**
 * @brief Renders a sprite object to the console using reverse-colored spaces.
 * 
 * @param spriteObj The sprite object to render to the console.
 * @param paletteNum The speific palette to use.
 */
void spriteToConsole(pSpr_t* spriteObj, int paletteNum);

#endif //MHPS_H_INCLUDED