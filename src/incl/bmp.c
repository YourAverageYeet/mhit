#include "universal.h"
#include "bmp.h"

// Error Variables

const char* notBMP = "Input is not a BMP file. Exiting...\n";

const char* noBMPV5 = "Bitmap does not contain a BITMAPV5HEADER-style DIB header. Exiting...\n";

const int EC_notBMP = 0xBBADF11E; // B(MP) Bad File

const int EC_noBMPV5 = 0xBAD04EAD; // Bad Head

// Function Variables

const char* validSig = "BM";

const char* compressionTypes[] = {
    "BI_RGB; Uncompressed",
    "BI_RLE8; 8bpp Run Length Encoding",
    "BI_RLE4; 4bpp Run Length Encoding",
    "BI_BITFIELDS; Uncompressed with bitmasks",
    "BI_JPEG; JPEG Compression",
    "BI_PNG; PNG Compression"
};

const char* renderIntents[] = {
    "LCS_GM_BUSINESS (Maintains Saturation)\n",
    "LCS_GM_GRAPHICS (Maintains Color)\n",
    "LCS_GM_IMAGES (Maintains Contrast)\n",
    "LCS_GM_ABS_COLORIMETRIC (Nearest Destination Color)\n"
};

// Functions

argbColor_t longToColor(uint32_t value){
    argbColor_t newColor = {0, 0, 0, 0};
    newColor.alpha = (uint8_t)((value & 0xFF000000) >> 24);
    newColor.red = (uint8_t)((value & 0xFF0000) >> 16);
    newColor.green = (uint8_t)((value & 0xFF00) >> 8);
    newColor.blue = value & 0xFF;
    return newColor;
}

uint32_t colorToLong(argbColor_t color){
    uint32_t value = 0;
    value |= ((uint32_t)color.blue) << 24;
    value |= ((uint32_t)color.green) << 16;
    value |= ((uint16_t)color.red) << 8;
    value |= color.alpha;
    return value;
}

void checkBMPFile(FILE* inputFile){
    fseek(inputFile, 0, SEEK_SET);
    char signature[3] = {0x00, 0x00, 0x00};
    fread(&signature, 1, 2, inputFile);
    if(strcmp(signature, validSig)){
        errorOut(notBMP, EC_notBMP);
    }
}

bmpHead_t* readBMPHeader(FILE* inputFile){
    checkBMPFile(inputFile);
    bmpHead_t* newHeader = malloc(sizeof(bmpHead_t));
    int headerSize = sizeof(bmpHead_t) / 2;
    uint16_t* wordBuff = malloc(headerSize * sizeof(uint16_t));
    fseek(inputFile, 0, SEEK_SET);
    fread(wordBuff, sizeof(uint16_t), headerSize, inputFile);
    newHeader->signature[0] = (uint8_t)((wordBuff[0] & 0xFF00) >> 8);
    newHeader->signature[1] = (uint8_t)(wordBuff[0] & 0xFF);
    uint32_t tmp = ((uint32_t)wordBuff[2] << 16) | wordBuff[1];
    newHeader->fileSize = tmp;
    newHeader->reserved1 = wordBuff[3];
    newHeader->reserved2 = wordBuff[4];
    tmp = ((uint32_t)wordBuff[6] << 16) | wordBuff[5];
    newHeader->offToPixels = tmp;
    free(wordBuff);
    return newHeader;
}

triplePoint_t readCIETriple(FILE* inputFile){
    fpos_t position = {};
    fgetpos(inputFile, &position);
    if(position.__pos != (14 + (15 * 4))){
        fseek(inputFile, (14 + (15 * 4)), SEEK_SET);
    }
    int32_t* colorCoords = malloc(sizeof(int32_t) * 9);
    triplePoint_t coords = {0};
    fread(colorCoords, sizeof(int32_t), 9, inputFile);
    coords.cie_red.coord_x = colorCoords[0];
    coords.cie_red.coord_y = colorCoords[1];
    coords.cie_red.coord_z = colorCoords[2];
    coords.cie_green.coord_x = colorCoords[3];
    coords.cie_green.coord_y = colorCoords[4];
    coords.cie_green.coord_z = colorCoords[5];
    coords.cie_blue.coord_x = colorCoords[6];
    coords.cie_blue.coord_y = colorCoords[7];
    coords.cie_blue.coord_z = colorCoords[8];
    free(colorCoords);
    return coords;
}

dibHead_t* readDIBHeader(FILE* inputFile){
    fpos_t position = {};
    fgetpos(inputFile, &position);
    if(position.__pos != 14){
        fseek(inputFile, 14, SEEK_SET);
    }
    fseek(inputFile, 14, SEEK_SET);
    uint32_t headSize = 0;
    fread(&headSize, sizeof(uint32_t), 1, inputFile);
    if(headSize != 0x7C){
        errorOut(noBMPV5, EC_noBMPV5);
    }
    dibHead_t* newDIB = malloc(sizeof(dibHead_t));
    newDIB->headerSize = headSize;
    uint32_t* longBuff = malloc(sizeof(uint32_t) * 14);
    fread(longBuff, sizeof(uint32_t), 14, inputFile);
    newDIB->imageWidth = (int32_t)longBuff[0];
    newDIB->imageHeight = (int32_t)longBuff[1];
    newDIB->planeCount = (uint16_t)(longBuff[2] & 0xFFFF);
    newDIB->bitsPerPixel = (uint16_t)(longBuff[2] >> 16);
    newDIB->compression = longBuff[3];
    newDIB->imageSize = longBuff[4];
    newDIB->pixPerMeterX = (int32_t)longBuff[5];
    newDIB->pixPerMeterY = (int32_t)longBuff[6];
    newDIB->colorTableSize = longBuff[7];
    newDIB->numImportant = longBuff[8];
    newDIB->bitmaskRed = longBuff[9];
    newDIB->bitmaskGreen = longBuff[10];
    newDIB->bitmaskBlue = longBuff[11];
    newDIB->bitmaskAlpha = longBuff[12];
    newDIB->colorSpace = longBuff[13];
    newDIB->colorEndpoints = readCIETriple(inputFile);
    fread(longBuff, sizeof(uint32_t), 6, inputFile);
    newDIB->gammaRed = longBuff[0];
    newDIB->gammaGreen = longBuff[1];
    newDIB->gammaBlue = longBuff[2];
    newDIB->renderIntent = longBuff[3];
    newDIB->iccData = longBuff[4];
    newDIB->iccSize = longBuff[5];
    newDIB->reserved = 0;
    free(longBuff);
    return newDIB;
}

bmpRawFile_t* createRawBMP(FILE* inputFile){
    bmpRawFile_t* newRaw = malloc(sizeof(bmpRawFile_t));
    newRaw->fileHeader = readBMPHeader(inputFile);
    newRaw->deviceHeader = readDIBHeader(inputFile);
    uint32_t colors = newRaw->deviceHeader->colorTableSize;
    if(colors){
        uint32_t* cLongs = malloc(sizeof(uint32_t) * colors);
        argbColor_t* colorLUT = malloc(sizeof(argbColor_t) * colors);
        for(int i = 0; i < colors; i++){
            colorLUT[i] = longToColor(cLongs[i]);
        }
        newRaw->colorTable = colorLUT;
        free(cLongs);
        free(colorLUT);
    } else {
        newRaw->colorTable = NULL;
    }
    int32_t iW = newRaw->deviceHeader->imageWidth;
    int32_t iH = newRaw->deviceHeader->imageHeight;
    uint32_t picOff = newRaw->fileHeader->offToPixels;
    uint64_t pixCount = iW * iH;
    uint32_t* pix = malloc(sizeof(uint32_t) * pixCount);
    uint32_t* rowBuff = malloc(sizeof(uint32_t) * iW);
    for(int r = (iH - 1); r >= 0; r--){
        int offset = picOff + (r * iW * sizeof(uint32_t));
        fseek(inputFile, offset, SEEK_SET);
        fread(rowBuff, sizeof(uint32_t), iW, inputFile);
        for(int c = 0; c < iW; c++){
            int pixelOff = ((iH - 1 - r) * iW) + c;
            pix[pixelOff] = rowBuff[c];
        }
    }
    newRaw->pixelArray = pix;
    if(newRaw->deviceHeader->iccData){
        uint8_t* icc = malloc(newRaw->deviceHeader->iccSize);
        fseek(inputFile, (14 + newRaw->deviceHeader->iccData), SEEK_SET);
        fread(icc, 1, newRaw->deviceHeader->iccSize, inputFile);
        newRaw->iccProfile = icc;
        free(icc);
    } else {
        newRaw->iccProfile = NULL;
    }
    free(rowBuff);
    return newRaw;
}

int checkBMPColorSpace(bmpRawFile_t* rawBMP, uint32_t style){
    if(rawBMP->deviceHeader->colorSpace == style){
        return TRUE;
    } else {
        return FALSE;
    }
}

void destroyRawBMP(bmpRawFile_t* bmp){
    free(bmp->fileHeader);
    free(bmp->deviceHeader);
    free(bmp->colorTable);
    free(bmp->pixelArray);
    free(bmp->iccProfile);
    free(bmp);
}

void bmpDetailsOut(bmpRawFile_t* bmp){
    puts("\n--- BMP Header ---");
    printf("File Size:\t%" PRIu32 " bytes\n", bmp->fileHeader->fileSize);
    printf("Offset to Pixel Data:\t%" PRIu32 " bytes\n",\
        bmp->fileHeader->offToPixels);
    puts("\n--- DIB Header ---");
    printf("Header Size:\t%" PRIu32 " bytes\n", bmp->deviceHeader->headerSize);
    printf("Image Width:\t%" PRId32 " pixels\n", bmp->deviceHeader->imageWidth);
    printf("Image Height:\t%" PRId32 " pixels\n",\
        bmp->deviceHeader->imageHeight);
    printf("Plane Count:\t%" PRIu32 "\n", bmp->deviceHeader->planeCount);
    printf("Bits per Pixel:\t%" PRIu32 "\n", bmp->deviceHeader->bitsPerPixel);
    printf("Compression:\t%" PRIu32 "\t(%s)\n", bmp->deviceHeader->compression,\
        compressionTypes[bmp->deviceHeader->compression]);
    printf("Uncompressed Image Size:\t%" PRIu32 " bytes\n",\
        bmp->deviceHeader->imageSize);
    printf("Pixels per Meter (X):\t%" PRIu32 " pixels per meter\n",\
        bmp->deviceHeader->pixPerMeterX);
    printf("Pixels per Meter (Y):\t%" PRIu32 " pixels per meter\n",\
        bmp->deviceHeader->pixPerMeterY);
    printf("Size of Color Table:\t%" PRIu32 " colors\n",\
        bmp->deviceHeader->colorTableSize);
    printf("Number of \"Important\" Colors:\t%" PRIu32 " colors\n",\
        bmp->deviceHeader->numImportant);
    printf("Red Channel Bitmask:\t0b");
    bitPrint(bmp->deviceHeader->bitmaskRed, 32);
    puts("");
    printf("Green Channel Bitmask:\t0b");
    bitPrint(bmp->deviceHeader->bitmaskGreen, 32);
    puts("");
    printf("Blue Channel Bitmask:\t0b");
    bitPrint(bmp->deviceHeader->bitmaskBlue, 32);
    puts("");
    printf("Alpha Channel Bitmask:\t0b");
    bitPrint(bmp->deviceHeader->bitmaskAlpha, 32);
    puts("");
    printf("Color Space:\t%" PRIu32 "\t", bmp->deviceHeader->colorSpace);
    switch(bmp->deviceHeader->colorSpace){
        case(STYLE_SRGB):
            puts("(sRGB)");
            break;
        case(STYLE_WINDOWS):
            puts("(Windows Colorspace)");
            break;
        case(STYLE_CALIBRATED):
            puts("(Calibrated)");
            break;
        case(STYLE_LINKED):
            puts("(Linked ICC Profile)");
            break;
        case(STYLE_EMBEDED):
            puts("(Embeded ICC Profile)");
            break;
        default:
            puts("(Invalid)");
            break;
    }
    puts("CIE Coloe Space Endpoints: (For use with \"Calibrated\")");
    puts("\tRed Point:");
    printf("\t\tX - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_red.coord_x);
    printf("\t\tY - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_red.coord_y);
    printf("\t\tZ - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_red.coord_z);
    puts("\tGreen Point:");
    printf("\t\tX - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_green.coord_x);
    printf("\t\tY - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_green.coord_y);
    printf("\t\tZ - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_green.coord_z);
    puts("\tBlue Point:");
    printf("\t\tX - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_blue.coord_x);
    printf("\t\tY - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_blue.coord_y);
    printf("\t\tZ - %" PRId32 "\n",\
        bmp->deviceHeader->colorEndpoints.cie_blue.coord_z);
    printf("Red Gamma:\t0x%" PRIX32 "\n", bmp->deviceHeader->gammaRed);
    printf("Green Gamma:\t0x%" PRIX32 "\n", bmp->deviceHeader->gammaGreen);
    printf("Blue Gamma:\t0x%" PRIX32 "\n", bmp->deviceHeader->gammaBlue);
    printf("Render Intent:\t%" PRIu32 "\t", bmp->deviceHeader->renderIntent);
    for(int i = 0; i < 4; i++){
        if(bmp->deviceHeader->renderIntent >> i){
            printf("%s", renderIntents[i]);
            break;
        }
    }
    printf("ICC Data Offset:\t%" PRIu32 " bytes\n", bmp->deviceHeader->iccData);
    printf("ICC Data Size:\t%" PRIu32 " bytes\n", bmp->deviceHeader->iccSize);
    puts("\n--- Pixel Data ---");
    for(int r = 0; r < bmp->deviceHeader->imageHeight; r++){
        int base = r * bmp->deviceHeader->imageWidth;
        for(int c = 0; c < bmp->deviceHeader->imageWidth; c++){
            printf("0x%08" PRIX32 " ", bmp->pixelArray[base + c]);
        }
        puts("");
    }
}
