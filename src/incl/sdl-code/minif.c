#include "minif.h"

// Variables

int capitals[] = {MINIF_A, MINIF_B, MINIF_C, MINIF_D, MINIF_E, MINIF_F, MINIF_G,
    MINIF_H, MINIF_I, MINIF_J, MINIF_K, MINIF_L, MINIF_M, MINIF_N, MINIF_O,
    MINIF_P, MINIF_Q, MINIF_R, MINIF_S, MINIF_T, MINIF_U, MINIF_V, MINIF_W,
    MINIF_X, MINIF_Y, MINIF_Z};

int lowers[] = {MINIF_A_LOWER, MINIF_B_LOWER, MINIF_C_LOWER, MINIF_D_LOWER,
    MINIF_E_LOWER, MINIF_F_LOWER, MINIF_G_LOWER, MINIF_H_LOWER, MINIF_I_LOWER,
    MINIF_J_LOWER, MINIF_K_LOWER, MINIF_L_LOWER, MINIF_M_LOWER, MINIF_N_LOWER,
    MINIF_O_LOWER, MINIF_P_LOWER, MINIF_Q_LOWER, MINIF_R_LOWER, MINIF_S_LOWER,
    MINIF_T_LOWER, MINIF_U_LOWER, MINIF_V_LOWER, MINIF_W_LOWER, MINIF_X_LOWER,
    MINIF_Y_LOWER, MINIF_Z_LOWER};

int numbers[] = {MINIF_NUM_0, MINIF_NUM_1, MINIF_NUM_2, MINIF_NUM_3,
    MINIF_NUM_4, MINIF_NUM_5, MINIF_NUM_6, MINIF_NUM_7, MINIF_NUM_8,
    MINIF_NUM_9};

int punct1[] = {MINIF_PUNCT_EX, MINIF_PUNCT_DQ, MINIF_PUNCT_HT, MINIF_PUNCT_DS,
    MINIF_PUNCT_PC, MINIF_PUNCT_AM, MINIF_PUNCT_QU, MINIF_PUNCT_LP,
    MINIF_PUNCT_RP, MINIF_PUNCT_AS, MINIF_PUNCT_PL, MINIF_PUNCT_CO,
    MINIF_PUNCT_DA, MINIF_PUNCT_PE, MINIF_PUNCT_FS};

int punct2[] = {MINIF_PUNCT_CL, MINIF_PUNCT_SC, MINIF_PUNCT_LT, MINIF_PUNCT_EQ,
    MINIF_PUNCT_GT, MINIF_PUNCT_QM, MINIF_PUNCT_AT};

int punct3[] = {MINIF_PUNCT_LB, MINIF_PUNCT_BS, MINIF_PUNCT_RB, MINIF_PUNCT_CT,
    MINIF_PUNCT_US, MINIF_PUNCT_AC};

int punct4[] = {MINIF_PUNCT_LC, MINIF_PUNCT_PI, MINIF_PUNCT_RC, MINIF_PUNCT_TD};

const char* invalChar = "Invalid character supplied to drawMinifCharacter(). \
Exiting...";

const int EC_invalChar = 0xBAD0C4A2; // BAD CHAR

char* minifTest1 = "the quick brown fox jumps over the lazy dog...";

char* minifTest2 = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG!";

char* minifTest3 = "Consciousness Credible?";

char* minifTest4 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char* minifTest5 = "abcdefghijklmnopqrstuvwxyz";

char* minifTest6 = "1234567890";

char* minifTest7 = "`~!@#$%^&*()-_=+,<.>/?;:'\"[{]}\\|";

// Functions

void drawMinifCharacter(vis_t* vis, objPos_t charPos, char c){
    if(c < 0x20 || c >= 0x7F){
        errorOut(invalChar, EC_invalChar);
    }
    uint16_t bits;
    if(c == 0x20){
        bits = 0;
    } else if(c >= 0x21 && c < 0x30){
        bits = punct1[c - 0x21];
    } else if(c >= 0x30 && c < 0x3A){
        bits = numbers[c - 0x30];
    } else if(c >= 0x3A && c < 0x41){
        bits = punct2[c - 0x3A];
    } else if(c >= 0x41 && c < 0x5B){
        bits = capitals[c - 0x41];
    } else if(c >= 0x5B && c < 0x61){
        bits = punct3[c - 0x5B];
    } else if(c >= 0x61 && c < 0x7B){
        bits = lowers[c - 0x61];
    } else {
        bits = punct4[c - 0x7B];
    }
    bits <<= 1;
    for(int row = 0; row < 5; row++){
        for(int col = 0; col < 3; col++){
            int state = bits & 0x8000;
            if(state){
                SDL_SetRenderDrawColor(vis->rend, 0xFF, 0xFF, 0xFF,
                    SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(vis->rend, 0x00, 0x00, 0x00,
                    SDL_ALPHA_OPAQUE);
            }
            SDL_RenderDrawPoint(vis->rend, (charPos.x_pos + col),
                (charPos.y_pos + row));
            bits <<= 1;
        }
    }
}

void minifString(vis_t* vis, objPos_t startPos, char* str){
    int len = strlen(str);
    SDL_SetRenderDrawColor(vis->rend, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    int x = startPos.x_pos - 1;
    int y = startPos.y_pos - 1;
    int w = (len * 3) + (len - 1) + 2;
    SDL_Rect box = {x, y, w, 7};
    SDL_RenderFillRect(vis->rend, &box);
    for(int i = 0; i < len; i++){
        objPos_t newPos = {(startPos.x_pos + (4 * i)), startPos.y_pos};
        drawMinifCharacter(vis, newPos, str[i]);
    }
}
