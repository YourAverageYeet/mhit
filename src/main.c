/**
 * @file main.c
 * @author Mike Hensley (mike2google.tamu.edu)
 * @brief Main source file for MHIT
 * @version 0.1
 * @date 2024-01-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "incl/universal.h"
#include "incl/mhps.h"
#include "incl/bmp.h"
#include "incl/util.h"

const char* modes[] = {
    "info",
    "display"
};

const int modeLen = sizeof(modes) / sizeof(modes[0]);

const char* invalMode = "Invalid mode supplied; use mhit --help to see valid modes. Exiting...\n";

const int EC_invalMode = 0xBADF00D;

char* getFileExtension(char* filePath){
    char* part = strtok(filePath, ".");
    char* prev = "";
    while(part != NULL){
        prev = part;
        part = strtok(NULL, ".");
    }
    return prev;
}

/**
 * @brief The `main` function of MHIT
 * 
 * @param argc The amount of arguments passed
 * @param argv The vectors to those arguments
 * @return int The program return value under normal circumstances
 */
int main(int argc, char* argv[]){
    int mode = stringWithinArray(argv[1], modes, modeLen);
    if(mode == -1){
        errorOut(invalMode, EC_invalMode);
    } else {
        FILE* fileStream = NULL;
        char* ext = NULL;
        switch(mode){
            case(0):
                fileStream = fopen(argv[2], "rb");
                ext = getFileExtension(argv[2]);
                spewInfo(fileStream, ext);
                break;
            case(1):
                fileStream = fopen(argv[2], "rb");
                ext = getFileExtension(argv[2]);
                spriteDisplay(fileStream, ext);
                break;
            default:
                errorOut(switchDef, EC_switchDef);
                break;
        }
    }
    return 0;
}
