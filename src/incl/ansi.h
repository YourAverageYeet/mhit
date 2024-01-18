/**
 * @file ansi.h
 * @author Mike Hensley (mike2@google.tamu.edu)
 * @brief Functions utilizing the ANSI control codes
 * @version 0.1
 * @date 2024-01-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef ANSI_H_INCLUDED
#define ANSI_H_INCLUDED

#include "universal.h"

/**
 * @brief The ANSI console reset sequence. Used in `reserTextColor()`.
 * 
 */
extern char* ANSI_RESET;

/**
 * @brief The ANSI codes for the base 16 colors supported by most consoles. Used by `setTextColorBase()`.
 * 
 */
extern char* ANSI_COLORS[];

/**
 * @brief A `printf()` compatable version of the 256-color ANSI code.
 * 
 */
extern char* ANSI_HIGHCOLOR;

/**
 * @brief A `printf()` compatable version of the ANSI truecolor code.
 * 
 */
extern char* ANSI_TRUECOLOR;

/**
 * @brief A `printf()` compatable skeleton of the ANSI character movement codes.
 * 
 */
extern char* ANSI_CUR_BASE;

/**
 * @brief Set the color of printed text using the 16 base colors.
 * 
 * @param pixColor The desired color's number (0 through 15).
 */
void setTextColorBase(uint8_t pixColor);

/**
 * @brief Set the color of printed text using the ANSI 256-color lookup table.
 * 
 * @param picColor The index of the desired color.
 */
void setTextColorHigh(uint8_t picColor);

/**
 * @brief Set the color of printed text using red, green and blue components.
 * 
 * @param red The amount of red desired in the color, from 0 to 255.
 * @param green The amount of green desired in the color, from 0 to 255.
 * @param blue The amount of blue desired in the color, from 0 to 255.
 */
void setTextColorTrue(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Resets used ANSI codes to default values.
 * 
 */
void ansiTextReset(void);

/**
 * @brief Moves the cursor a given number of spaces in a given direction.
 * 
 * @param dir The desired direction for cursor movement.
 * @param amount The amount of character spaces to move.
 */
void moveCursor(uint8_t dir, uint8_t amount);

/**
 * @brief Moves the cursor up a given number of lines, placing the cursor at the start of the line.
 * 
 * @param amount The number of lines to move.
 */
void cursorLinesUp(int amount);

/**
 * @brief Moves the cursor down a given number of lines, placing the cursor at the start of the line.
 * 
 * @param amount The number of lines to move.
 */
void cursorLinesDown(int amount);

#endif // ANSI_H_INCLUDED
