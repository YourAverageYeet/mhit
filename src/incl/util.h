/**
 * @file util.h
 * @author Mike Hensley (mike2@tamu.edu)
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

// Variables

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

// Functions

/**
 * @brief Prints file information to console. Supports the formats listed in
 * `validExts`.
 * 
 * @param stream The file to report info for.
 * @param extension The extenxion of the supplied file.
 */
void spewInfo(FILE* stream, char* extension);

/**
 * @brief Displays a sprite to the console.
 * 
 * @param stream The sprite file to display.
 * @param extension The extenxion of the supplied file.
 */
void spriteDisplay(FILE* stream, char* extension);

// Help Text Variables

/**
 * @brief The program's main help text. Output when the specified mode is
 * `help` with no other arguemnts.
 * 
 */
extern const char* helpText;

/**
 * @brief List of variables that contain help for a specied mode. Use when the
 * specified mode is `help [mode]`.
 * 
 */
extern const char* hModeTexts[];

#endif // UTIL_H_DEFINED