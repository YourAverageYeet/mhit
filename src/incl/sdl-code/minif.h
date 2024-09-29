/**
 * @file minif.h
 * @author Mike Hensley (mike2@tamu.edu)
 * @brief Miniature font for SDL2
 * @version 1.0
 * @date 2024-05-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef MINIF_H_INCLUDED
#define MINIF_H_INCLUDED

#include "base-sdl.h"
#include "../universal.h"

// Uppercase Letters

#define MINIF_A         11245   ///< Capital "A"
#define MINIF_B         27566   ///< Capital "B"
#define MINIF_C         14627   ///< Capital "C"
#define MINIF_D         27502   ///< Capital "D"
#define MINIF_E         31143   ///< Capital "E"
#define MINIF_F         31140   ///< Capital "F"
#define MINIF_G         14699   ///< Capital "G"
#define MINIF_H         23533   ///< Capital "H"
#define MINIF_I         29847   ///< Capital "I"
#define MINIF_J         29290   ///< Capital "J"
#define MINIF_K         23469   ///< Capital "K"
#define MINIF_L         18727   ///< Capital "L"
#define MINIF_M         24557   ///< Capital "M"
#define MINIF_N         27501   ///< Capital "N"
#define MINIF_O         31599   ///< Capital "O"
#define MINIF_P         27556   ///< Capital "P"
#define MINIF_Q         31611   ///< Capital "Q"
#define MINIF_R         27565   ///< Capital "R"
#define MINIF_S         14478   ///< Capital "S"
#define MINIF_T         29842   ///< Capital "T"
#define MINIF_U         23407   ///< Capital "U"
#define MINIF_V         23402   ///< Capital "V"
#define MINIF_W         23549   ///< Capital "W"
#define MINIF_X         23213   ///< Capital "X"
#define MINIF_Y         23186   ///< Capital "Y"
#define MINIF_Z         29351   ///< Capital "Z"

// Lowercase Letters

#define MINIF_A_LOWER   171     ///< Lowercase "a"
#define MINIF_B_LOWER   18862   ///< Lowercase "b"
#define MINIF_C_LOWER   227     ///< Lowercase "c"
#define MINIF_D_LOWER   4843    ///< Lowercase "d"
#define MINIF_E_LOWER   503     ///< Lowercase "e"
#define MINIF_F_LOWER   13778   ///< Lowercase "f"
#define MINIF_G_LOWER   10959   ///< Lowercase "g"
#define MINIF_H_LOWER   18861   ///< Lowercase "h"
#define MINIF_I_LOWER   1042    ///< Lowercase "i"
#define MINIF_J_LOWER   8340    ///< Lowercase "j"
#define MINIF_K_LOWER   18805   ///< Lowercase "k"
#define MINIF_L_LOWER   9361    ///< Lowercase "l"
#define MINIF_M_LOWER   509     ///< Lowercase "m"
#define MINIF_N_LOWER   493     ///< Lowercase "n"
#define MINIF_O_LOWER   495     ///< Lowercase "o"
#define MINIF_P_LOWER   3444    ///< Lowercase "p"
#define MINIF_Q_LOWER   1881    ///< Lowercase "q"
#define MINIF_R_LOWER   228     ///< Lowercase "r"
#define MINIF_S_LOWER   214     ///< Lowercase "s"
#define MINIF_T_LOWER   11922   ///< Lowercase "t"
#define MINIF_U_LOWER   367     ///< Lowercase "u"
#define MINIF_V_LOWER   362     ///< Lowercase "v"
#define MINIF_W_LOWER   383     ///< Lowercase "w"
#define MINIF_X_LOWER   341     ///< Lowercase "x"
#define MINIF_Y_LOWER   378     ///< Lowercase "y"
#define MINIF_Z_LOWER   403     ///< Lowercase "z"

// Numerals

#define MINIF_NUM_1     11415   ///< Arabic Numeral "1"
#define MINIF_NUM_2     25255   ///< Arabic Numeral "2"
#define MINIF_NUM_3     25230   ///< Arabic Numeral "3"
#define MINIF_NUM_4     23497   ///< Arabic Numeral "4"
#define MINIF_NUM_5     31118   ///< Arabic Numeral "5"
#define MINIF_NUM_6     10735   ///< Arabic Numeral "6"
#define MINIF_NUM_7     29348   ///< Arabic Numeral "7"
#define MINIF_NUM_8     10922   ///< Arabic Numeral "8"
#define MINIF_NUM_9     31689   ///< Arabic Numeral "9"
#define MINIF_NUM_0     11114   ///< Arabic Numeral "0"

// Punctuation

#define MINIF_PUNCT_PE  18      ///< Period
#define MINIF_PUNCT_CO  20      ///< Comma
#define MINIF_PUNCT_EX  9346    ///< Exclamation Point
#define MINIF_PUNCT_QM  29378   ///< Question Mark
#define MINIF_PUNCT_AT  31591   ///< At Sign
#define MINIF_PUNCT_HT  24445   ///< Hashtag / Pound
#define MINIF_PUNCT_DS  9906    ///< Dollar Sign
#define MINIF_PUNCT_PC  21157   ///< Percent
#define MINIF_PUNCT_CT  10752   ///< Caret
#define MINIF_PUNCT_AM  9714    ///< Ampersand
#define MINIF_PUNCT_AS  21824   ///< Asterisk
#define MINIF_PUNCT_LP  10530   ///< Left Parenthesis
#define MINIF_PUNCT_RP  8778    ///< Right Parenthesis
#define MINIF_PUNCT_LT  5393    ///< Less Than
#define MINIF_PUNCT_GT  17492   ///< Greater Than
#define MINIF_PUNCT_LB  31015   ///< Left Bracket
#define MINIF_PUNCT_RB  29263   ///< Right Bracket
#define MINIF_PUNCT_LC  13587   ///< Left (Curly) Brace
#define MINIF_PUNCT_RC  25686   ///< Right (Curly) Brace
#define MINIF_PUNCT_BS  18569   ///< Backslash
#define MINIF_PUNCT_PI  9362    ///< Pipe
#define MINIF_PUNCT_FS  4772    ///< Forward Slash
#define MINIF_PUNCT_AC  17408   ///< Acute
#define MINIF_PUNCT_TD  2184    ///< Tilde
#define MINIF_PUNCT_DA  448     ///< Dash / Minus
#define MINIF_PUNCT_US  7       ///< Underscore
#define MINIF_PUNCT_EQ  3640    ///< Equals Sign
#define MINIF_PUNCT_PL  1488    ///< Plus
#define MINIF_PUNCT_CL  9234    ///< Colon
#define MINIF_PUNCT_SC  9236    ///< Semicolon
#define MINIF_PUNCT_DQ  23040   ///< Double Quote
#define MINIF_PUNCT_QU  9216    ///< Single Quote

// Variables

extern char* minifTest1;    ///< Test string 1

extern char* minifTest2;    ///< Test string 2

extern char* minifTest3;    ///< Test string 3

extern char* minifTest4;    ///< Test string 4

extern char* minifTest5;    ///< Test string 5

extern char* minifTest6;    ///< Test string 6

extern char* minifTest7;    ///< Test string 7

// Functions

void drawMinifCharacter(vis_t* vis, objPos_t charPos, char c);

void minifString(vis_t* vis, objPos_t startPos, char* str);

#endif // MINIF_H_INCLUDED
