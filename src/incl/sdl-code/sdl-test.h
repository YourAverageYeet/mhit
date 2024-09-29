/**
 * @file sdl-test.h
 * @author Mike Hensley (mike2@tamu.edu)
 * @brief Function used for the `vis-test` mode
 * @version 0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SDL_TEST_INCLUDEDED
#define SDL_TEST_INCLUDEDED

#include "base-sdl.h"
#include "minif.h"

/**
 * @brief Function used for the `vis-test` mode
 * 
 * @param testChar The character representing the test to be run
 * @param vis The visualizer to use
 */
void unifiedSDLTest(char testChar, vis_t* vis);

#endif // SDL_TEST_INCLUDED