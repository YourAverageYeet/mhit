/**
 * @file universal.h
 * @author Mike Hensley (mike2@tamu.edu)
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <errno.h>

// Self-written includes

#include "ansi.h"

// Funny business

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

/**
 * @brief Checks if a scanf() call completed in an expected manner and addresses
 * the errors if present.
 * 
 * @param scanVal The return value of a scanf() call.
 */
void scanCheck(int scanVal);

/**
 * @brief Checks if a fread() call completed in an expected manner and addresses
 * the errors if present.
 * 
 * @param actual The actual number of objects read (fread() return value). 
 * @param expected The expected number of objects read ("count" in fread()).
 * @param context The name of the function that fread was called within.
 */
void readCheck(size_t actual, size_t expected, char* context, FILE* stream);

/**
 * @brief Flips the endianness of a 16-bit number.
 * 
 * @param value The 16-bit number to swap.
 * @return uint16_t The endian-swapped 16-bit number.
 */
uint16_t byteSwap16(uint16_t value);

/**
 * @brief Flips the endianness of a 32-bit number.
 * 
 * @param value The 32-bit number to swap.
 * @return uint32_t The endian-swapped 32-bit number.
 */
uint32_t byteSwap32(uint32_t value);

// /**
//  * @brief Flips the endianness of a 64-bit number.
//  * 
//  * @param value The 64-bit number to swap.
//  * @return uint64_t The endian-swapped 64-bit number.
//  */
// uint64_t byteSwap64(uint64_t value);

#endif //UNIVERSAL_H_DEFINED