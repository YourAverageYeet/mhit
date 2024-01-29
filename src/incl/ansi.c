#include "ansi.h"
#include "universal.h"

// Variables

const int EC_invalColor = 0xBADC0102; // Bad color

const char* invalColor = "Invalid pixel color found in sprite. Exiting...\n";

const char* ANSI_RESET = "\033[0m";

const char* ANSI_COLORS[] = {
    "\033[40m",             // Black
    "\033[41m",             // Red
    "\033[42m",             // Green
    "\033[43m",             // Yellow
    "\033[44m",             // Blue
    "\033[45m",             // Magenta
    "\033[46m",             // Cyan
    "\033[47m",             // Light Gray
    "\033[100m",             // Dark Gray
    "\033[101m",             // Bright Red
    "\033[102m",             // Bright Green
    "\033[103m",             // Bright Yellow
    "\033[104m",             // Bright Blue
    "\033[105m",             // Bright Magenta
    "\033[106m",             // Bright Cyan
    "\033[107m"              // White
};

const char* ANSI_HIGHCOLOR = "\033[48;5;%" PRIu8;

const char* ANSI_TRUECOLOR = "\033[48;2;%" PRIu8 ";%" PRIu8 ";%" PRIu8 "m";

const char* ANSI_CUR_BASE = "\033[%d%c";

// Functions

void setTextColorBase(uint8_t pixColor){
    if(pixColor >= 16){
        errorOut(invalColor, EC_invalColor);
    } else {
        printf("%s", ANSI_COLORS[pixColor]);
    }
}

void setTextColorHigh(uint8_t pixColor){
    printf(ANSI_HIGHCOLOR, pixColor);
}

void setTextColorTrue(uint8_t red, uint8_t green, uint8_t blue){
    printf(ANSI_TRUECOLOR, red, green, blue);
}

void ansiTextReset(void){
    printf("%s", ANSI_RESET);
}

void moveCursor(uint8_t dir, uint8_t amount){
    dir %= 4;
    switch(dir){
        case 0:
            printf(ANSI_CUR_BASE, amount, 'A');
            break;
        case 1:
            printf(ANSI_CUR_BASE, amount, 'B');
            break;
        case 2:
            printf(ANSI_CUR_BASE, amount, 'C');
            break;
        case 3:
            printf(ANSI_CUR_BASE, amount, 'D');
            break;
        default:
            puts("This condition sould be unreachable!");
            break;
    }
}

void cursorLinesUp(int amount){
    printf("\033[%dF", amount);
}

void cursorLinesDown(int amount){
    printf("\033[%dE", amount);
}
