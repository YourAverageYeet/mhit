/**
 * @file universal.h
 * @author your name (you@domain.com)
 * @brief Includes, definitions, variables, and functions accessable to all
 * files.
 * @version 0.1
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef UNIVERSAL_H_DEFINED
#define UNIVERSAL_H_DEFINED

#define TRUE    1
#define FALSE   0

// Library includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <SDL2/SDL.h>
#include <sys/stat.h>

// Self-written includes

#include "ansi.h"

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#define stat _stat
#else
#include <unistd.h>
#endif

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

/**
 * @brief Checks if a files exists.
 * 
 * @param path The file to check for.
 */
void checkFileExists(char* path);

/**
 * @brief Checks if a array if completely filled with a single byte.
 * 
 * @param array The array to check.
 * @param value The value to check for.
 * @param size The size of the submitted array.
 * @return int If 1, then then `array` is full of bytes equal to `value`.
 */
int checkArrayFull_byte(uint8_t* array, uint8_t value, int size);

/**
 * @brief Check if a given path is a directory.
 * 
 * @param path The path to check.
 */
void checkIfDirectory(char* path);

#endif //UNIVERSAL_H_DEFINED