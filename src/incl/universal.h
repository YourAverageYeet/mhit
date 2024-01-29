#ifndef UNIVERSAL_H_DEFINED
#define UNIVERSAL_H_DEFINED

#define TRUE    1
#define FALSE   0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <SDL2/SDL.h>
#include "ansi.h"

/**
 * @brief String notifying of an out-of-memory error; for use with `errorOut()`.
 * 
 */
extern const char* noMem;

/**
 * @brief Exit code for the `noMem` condition.
 * 
 */
extern const int EC_noMem;

/**
 * @brief String notifying of a switch that reached it's default condition; for
 * use with `errorOut()`.
 * 
 */
extern const char* switchDef;

/**
 * @brief Exit code for the `switchDef` condition.
 * 
 */
extern const int EC_switchDef;

/**
 * @brief Prints an error message and exits.
 * 
 * @param msg The message to print.
 * @param ec The error code to exit with.
 */
void errorOut(const char* msg, int ec);

/**
 * @brief Prints a specified amount of bits to the console. 
 * 
 * @param value The reference number
 * @param bits Amount of bits to print.
 */
void bitPrint(uint64_t value, int bits);

/**
 * @brief Checks if a given string is within an array of strings.
 * 
 * @param str The string to check for.
 * @param array The array to search within.
 * @param arrayLength The length of the given array.
 * @return int The index of the string if found or `-1` if not found.
 */
int stringWithinArray(char* str, const char** array, int arrayLength);

#endif //UNIVERSAL_H_DEFINED