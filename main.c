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

int main(int argc, char* argv[]){
    for(int i = 0; i < argc; i++){
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}
