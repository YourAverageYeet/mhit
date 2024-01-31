/**
 * @file util.h
 * @author your name (you@domain.com)
 * @brief Functions to be called from `main()`.
 * @version 0.1
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef UTIL_H_DEFINED
#define UTIL_H_DEFINED

#include "universal.h"
#include "mhps.h"
#include "bmp.h"

/**
 * @brief Valid extensions for use with `spewInfo()`.
 * 
 */
extern const char* validInfoExts[];

/**
 * @brief Valid extensions for use with `displaySprite()`.
 * 
 */
extern const char* validDisplyExts[];

/**
 * @brief Prints file information to console. Supports the formats listed in
 * `validExts`.
 * 
 * @param stream 
 * @param extension 
 */
void spewInfo(FILE* stream, char* extension);

void spriteDisplay(FILE* stream, char* extension);

#endif // UTIL_H_DEFINED