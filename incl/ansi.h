#ifndef ANSI_H_INCLUDED
#define ANSI_H_INCLUDED

#include "universal.h"

extern char* ANSI_RESET;

extern char* ANSI_COLORS[];

extern char* ANSI_TRUECOLOR;

extern char* ANSI_CUR_BASE;

void setTextColorBase(uint8_t pixColor);

void setTextColorTrue(uint8_t red, uint8_t green, uint8_t blue);

void resetTextColor(void);

void moveCursor(uint8_t dir, uint8_t amount);

void cursorLinesUp(int amount);

void cursorLinesDown(int amount);

#endif // ANSI_H_INCLUDED
