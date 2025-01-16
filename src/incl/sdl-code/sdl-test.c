/**
 * @file sdl-test.c
 * @author Mike Hensley (mike2@tamu.edu)
 * @brief 
 * @version 1.0
 * @date 2024-06-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../universal.h"
#include "sdl-test.h"

const char* invalTest = "Invalid SDL testing mode supplied. Use \
\"./mhit help vis-test\" to see valid tests.\n";

const int EC_invalTest = 0xBAD7E57; // Bad Test

void unifiedSDLTest(char testChar, vis_t* vis){
    int exitVar = 0;
    switch(testChar){
        case('A'):
            printf("Running Test %c...\n", testChar);
            while(!exitVar){
                renderTestColor(vis);
                checkSDLExit(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('B'):
            printf("Running Test %c...\n", testChar);
            while(!exitVar){
                drawTestBox(vis);
                checkSDLExit(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('C'):
            printf("Running Test %c...\n", testChar);
            while(!exitVar){
                drawTestLine(vis);
                checkSDLExit(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('D'):
            printf("Running Test %c...\n", testChar);
            while(!exitVar){
                drawTestPoint(vis);
                checkSDLExit(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('E'):
            printf("Running Test %c...\n", testChar);
            objPos_t APos = {0, 0};
            objPos_t bPos = {10, 0};
            objPos_t Pos3 = {20, 0};
            objPos_t ExPos = {30, 0};
            objPos_t CoPos = {50, 0};
            objPos_t LbPos = {60, 0};
            objPos_t LcPos = {80, 0};
            objPos_t strPos1 = {1, 51};
            objPos_t strPos2 = {1, 61};
            objPos_t strPos3 = {2, 71};
            objPos_t strPos4 = {2, 91};
            objPos_t strPos5 = {3, 111};
            objPos_t strPos6 = {3, 141};
            objPos_t strPos7 = {4, 171};
            while(!exitVar){
                SDL_SetRenderDrawColor(vis->rend, 0x00,
                0x00, 0xFF, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(vis->rend);
                drawMinifCharacter(vis, APos, 'A', 1);
                drawMinifCharacter(vis, bPos, 'b', 2);
                drawMinifCharacter(vis, Pos3, '3', 1);
                drawMinifCharacter(vis, ExPos, '!', 3);
                drawMinifCharacter(vis, CoPos, ':', 1);
                drawMinifCharacter(vis, LbPos, '[', 4);
                drawMinifCharacter(vis, LcPos, '{', 1);
                minifString(vis, strPos1, minifTest1, 1);
                minifString(vis, strPos2, minifTest2, 1);
                minifString(vis, strPos3, minifTest3, 2);
                minifString(vis, strPos4, minifTest4, 2);
                minifString(vis, strPos5, minifTest5, 3);
                minifString(vis, strPos6, minifTest6, 3);
                minifString(vis, strPos7, minifTest7, 4);
                checkSDLExit(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        default:
            errorOut(invalTest, EC_invalTest);
    }
}
