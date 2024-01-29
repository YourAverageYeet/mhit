#include "universal.h"

const char* noMem = "Not enough memory error. Exiting...\n";

const int EC_noMem = 0x4B1D0DA1; // Forbid day

const char* switchDef = "Unexpectedswitch default reached. Exiting...\n";

const int EC_switchDef = 0xBADDA1; // Bad day

void errorOut(const char* msg, int ec){
    fprintf(stderr, "%s", msg);
    exit(ec);
}

void bitPrint(uint64_t value, int bitCount){
    moveCursor(CUR_RIGHT, (bitCount - 1));
    for(int i = 0; i < bitCount; i++){
        if(value & 0x01){
            printf("%c", '1');
        } else {
            printf("%c", '0');
        }
        moveCursor(CUR_LEFT, 2);
        value >>= 1;
    }
    moveCursor(CUR_RIGHT, (bitCount - 1));
}

int stringWithinArray(char* str, const char** array, int arrayLength){
    for(int i = 0; i < arrayLength; i++){
        if(!strcmp(str, array[i])){
            return i;
        }
    }
    return -1;
}
