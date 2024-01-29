#ifndef UTIL_H_DEFINED
#define UTIL_H_DEFINED

#include "universal.h"
#include "mhps.h"
#include "bmp.h"

/**
 * @brief Valid Extensions for use with `spewInfo()`.
 * 
 */
extern const char* validExts[];

/**
 * @brief Prints file information to console. Supports the formats listed in
 * `validExts`.
 * 
 * @param stream 
 * @param extension 
 */
void spewInfo(FILE* stream, char* extension);

#endif // UTIL_H_DEFINED