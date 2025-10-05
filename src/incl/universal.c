#include "universal.h"

const char* noMem = "Not enough memory error. Exiting...\n";

const int EC_noMem = 0x4B1D0DA1;    // Forbid day

const char* switchDef = "Unexpected switch default reached. Exiting...\n";

const int EC_switchDef = 0xBADDA1;  // Bad day

const char* noFile = "The object \"%s\" does not exist. Exiting...\n";

const int EC_noFile = 0x0BADF11E;   // Bad File

const char* notDir = "The supplied path \"%s\" is not a directory. \
Exiting...\n";

const int EC_notDir = 0xD12BAD;     // Dir Bad

const char* badScan0 = "scanf() unexpectedly read 0 objects; Exiting...";

const char* badScanEOF = "scanf() unexpectedly reached the end of the supplied \
file; Exiting...";

const int EC_badScan = 0xBAD5CAF;   // Bad Sca(n)f

const char* badReadEOF = "fread() encountered an unexpected EOF; call context \
was within %s().";

const char* badReadError = "An error was encountered in %s() while calling \
fread";

const char* badReadUnknown = "An unknown error occured within a call to \
fread() in %s(). How did we get here?";

const int EC_badRead = 0xBAD2EAD;   // Bad Read

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

void checkFileExists(char* path){
    if(access(path, F_OK)){
        fprintf(stderr, noFile, path);
        errorOut("", EC_noFile);
    }
}

int checkArrayFull_byte(uint8_t* array, uint8_t value, int size){
    for(int i = 0; i < size; i++){
        if(array[i] != value){
            return FALSE;
        }
    }
    return TRUE;
}

void checkIfDirectory(char* path){
    struct stat s;
    int statOut = stat(path, &s);
    if(statOut == -1){
        fprintf(stderr, noFile, path);
        errorOut("", EC_noFile);
    }
    if(!S_ISDIR(s.st_mode)){
        errorOut(notDir, EC_notDir);
    }
}

void scanCheck(int scanVal){
    if(!scanVal){
        errorOut(badScan0, EC_badScan);
    } else if(scanVal == EOF){
        errorOut(badScanEOF, EC_badScan);
    }
}

void readCheck(size_t actual, size_t expected, char* context, FILE* stream){
    if(actual != expected){
        if(feof(stream)){
            fprintf(stderr, badReadEOF, context);
        } else if(ferror(stream)){
            fprintf(stderr, badReadError, context);
            perror("()");
        } else {
            fprintf(stderr, badReadUnknown, context);
        }
        errorOut("Exiting...", EC_badRead);
    }
}

uint16_t byteSwap16(uint16_t value){
    uint8_t tmp = (uint8_t)(value >> 8);
    value <<= 8;
    value |= tmp;
    return value;
}

uint32_t byteSwap32(uint32_t value){
    uint16_t topHalf = (uint16_t)(value >> 16);
    uint16_t botHalf = (uint16_t)(value & 0xFFFF);
    value = byteSwap16(botHalf);
    value <<= 16;
    value |= byteSwap16(topHalf);
    return value;
}

// uint64_t byteSwap64(uint64_t value){
//     uint32_t topHalf = (uint32_t)(value >> 32);
//     uint32_t botHalf = (uint32_t)(value & 0xFFFFFFFF);
//     value = byteSwap32(botHalf);
//     value <<= 32;
//     value |= byteSwap32(topHalf);
//     return value;
// }
