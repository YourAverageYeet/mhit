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

#include "incl/sdl-code/base-sdl.h"
#include "incl/sdl-code/minif.h"
#include "incl/sdl-code/sdl-test.h"
#include "incl/sdl-code/disp-mode/mhps-disp.h"

#define M_MAJOR_VERSION 1
#define M_MINOR_VERSION 0
#define M_PATCH_VERSION 0

const char* modes[] = {
    "info",
    "cons",
    "convert",
    "help",
    "version",
    "vis-test",
    "display"
};

const int modeLen = sizeof(modes) / sizeof(modes[0]);

const char* invalMode = "Invalid mode supplied. Use \"./mhit help\" to see \
valid modes. Exiting...\n";

const int EC_invalMode = 0xBADF00D; // Bad Food

const char* noVisTest = "No test supplied to vis-test. Please use \
\"./mhit help vis-test\" to see valid tests.\n";

const int EC_noVisTest = 0x2E207E57; // Zero Test

const char* noDispFile = "No file supplied to display. Exiting...\n";

const int EC_noDispFile = 0x2E20F11E; // Zero File

const char* askSave = "Would you like to save this data? [Y/N]\n-> ";

const char* askName1 = "\nPlease enter the path to where you want to save this \
file.\nPlease limit your path to %d characters.\n-> ";

const char* askName2 = " %[^\n]s";

char savePath[1029];

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
    if(argc == 1){
        printf("%s\n", helpText);
        return 0;
    }
    int mode = stringWithinArray(argv[1], modes, modeLen);
    if(mode == -1){
        errorOut(invalMode, EC_invalMode);
    } else {
        FILE* fileStream = NULL;
        char* ext = NULL;
        switch(mode){
            case(0):
                checkFileExists(argv[2]);
                fileStream = fopen(argv[2], "rb");
                ext = getFileExtension(argv[2]);
                spewInfo(fileStream, ext);
                fclose(fileStream);
                break;
            case(1):
                checkFileExists(argv[2]);
                fileStream = fopen(argv[2], "rb");
                ext = getFileExtension(argv[2]);
                spriteDisplay(fileStream, ext);
                fclose(fileStream);
                break;
            case(2):
                checkFileExists(argv[2]);
                FILE* skelStream = fopen(argv[2], "rb");
                checkFileExists(argv[3]);
                FILE* palsStream = fopen(argv[3], "rb");
                bmpRawFile_t* skel = createRawBMP(skelStream);
                bmpRawFile_t* pals = createRawBMP(palsStream);
                pSpr_t* conv = rawBMPsToSprite(skel, pals);
                fclose(skelStream);
                fclose(palsStream);
                puts("\nCreated the following data:");
                displaySpriteData(conv);
                printf("%s", askSave);
                char c = (char)getchar();
                if(c != 'y' && c != 'Y'){
                    puts("\nExiting...");
                    destroySpriteObj(conv);
                    return 0;
                }
                printf(askName1, ((sizeof(savePath) / sizeof(savePath[0])) -\
                                    5));
                scanf(askName2, &savePath);
                spriteToFile(conv, savePath);
                destroySpriteObj(conv);
                break;
            case(3):
                if(argv[2]){
                    int hMode = stringWithinArray(argv[2], modes, modeLen);
                    printf("%s\n", hModeTexts[hMode]);
                } else {
                    printf("%s\n", helpText);
                }
                break;
            case(4):
                printf("\nMHIT Version: %d.%d.%d\n", M_MAJOR_VERSION,\
                        M_MINOR_VERSION, M_PATCH_VERSION);
                printf("Compiled with GCC %d.%d.%d\n", __GNUC__,\
                        __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
                printf("Compiled on %s at %s\n\n", __DATE__, __TIME__);
                break;
            case(5):
                puts("Starting SDL...");
                vis_t* vis_test = generateSDLVisualizer();
                if(argc != 3){
                    destroySDLVisualizer(vis_test);
                    errorOut(noVisTest, EC_noVisTest);
                }
                char test = argv[2][0];
                printf("Running Test: %c\n", test);
                unifiedSDLTest(test, vis_test);
                puts("Exiting SDL...");
                destroySDLVisualizer(vis_test);
                SDL_Quit();
                break;
            case(6):
                puts("Starting SDL...");
                vis_t* visualizer = generateSDLVisualizer();
                puts("SDL Started!");
                if(argc != 3){
                    destroySDLVisualizer(visualizer);
                    errorOut(noDispFile, EC_noDispFile);
                }
                checkFileExists(argv[2]);
                displayMHPS(visualizer, argv[2]);
                puts("Exiting SDL...");
                destroySDLVisualizer(visualizer);
                break;
            default:
                errorOut(switchDef, EC_switchDef);
                break;
        }
    }
    return 0;
}
