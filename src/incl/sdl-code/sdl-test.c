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
            while(!exitVar){
                renderTestColor(vis);
                handleSDLInput(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('B'):
            while(!exitVar){
                drawTestBox(vis);
                handleSDLInput(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('C'):
            while(!exitVar){
                drawTestLine(vis);
                handleSDLInput(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('D'):
            while(!exitVar){
                drawTestPoint(vis);
                handleSDLInput(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        case('E'):
            objPos_t APos = {0, 0};
            objPos_t bPos = {10, 0};
            objPos_t Pos3 = {20, 0};
            objPos_t ExPos = {30, 0};
            objPos_t CoPos = {40, 0};
            objPos_t LbPos = {50, 0};
            objPos_t LcPos = {60, 0};
            objPos_t strPos1 = {1, 11};
            objPos_t strPos2 = {1, 21};
            objPos_t strPos3 = {1, 31};
            objPos_t strPos4 = {1, 41};
            objPos_t strPos5 = {1, 51};
            objPos_t strPos6 = {1, 61};
            objPos_t strPos7 = {1, 71};
            while(!exitVar){
                SDL_SetRenderDrawColor(vis->rend, 0x00,
                0x00, 0xFF, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(vis->rend);
                drawMinifCharacter(vis, APos, 'A');
                drawMinifCharacter(vis, bPos, 'b');
                drawMinifCharacter(vis, Pos3, '3');
                drawMinifCharacter(vis, ExPos, '!');
                drawMinifCharacter(vis, CoPos, ':');
                drawMinifCharacter(vis, LbPos, '[');
                drawMinifCharacter(vis, LcPos, '{');
                minifString(vis, strPos1, minifTest1);
                minifString(vis, strPos2, minifTest2);
                minifString(vis, strPos3, minifTest3);
                minifString(vis, strPos4, minifTest4);
                minifString(vis, strPos5, minifTest5);
                minifString(vis, strPos6, minifTest6);
                minifString(vis, strPos7, minifTest7);
                handleSDLInput(&exitVar);
                showScreen(vis);
                SDL_Delay(16);
            }
            break;
        default:
            errorOut(invalTest, EC_invalTest);
    }
}
