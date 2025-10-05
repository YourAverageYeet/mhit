#include "universal.h"
#include "bmp.h"

// Error Variables

const char* notBMP = "Input is not a BMP file. Exiting...\n";

const char* noBMPV5 = "Bitmap does not contain a BITMAPV5HEADER-style\
 DIB header. Exiting...\n";

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
    value |= ((uint32_t)color.alpha) << 24;
    value |= ((uint32_t)color.red) << 16;
    value |= ((uint16_t)color.green) << 8;
    value |= color.blue;
    return value;
}

void checkBMPFile(FILE* inputFile){
    fseek(inputFile, 0, SEEK_SET);
    char signature[3] = {0x00, 0x00, 0x00};
    int read = fread(&signature, 1, 2, inputFile);
    readCheck(read, 2, "checkBMPFile", inputFile);
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
    int read = fread(wordBuff, sizeof(uint16_t), headerSize, inputFile);
    readCheck(read, headerSize, "readBMPHeader", inputFile);
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
    #ifndef _WIN32
    fpos_t position = {0, {0, {0}}};    // *NIX
    #else
    fpos_t position = 0;                // Windows
    #endif
    fgetpos(inputFile, &position);
    if(position.__pos != (14 + (15 * 4))){
        fseek(inputFile, (14 + (15 * 4)), SEEK_SET);
    }
    int32_t* colorCoords = malloc(sizeof(int32_t) * 9);
    triplePoint_t coords = {0};
    int read = fread(colorCoords, sizeof(int32_t), 9, inputFile);
    readCheck(read, 9, "readCIETriple", inputFile);
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
    #ifndef _WIN32
    fpos_t position = {0, {0, {0}}};    // *NIX
    #else
    fpos_t position = 0;                // Windows
    #endif
    fgetpos(inputFile, &position);
    if(position.__pos != 14){
        fseek(inputFile, 14, SEEK_SET);
    }
    fseek(inputFile, 14, SEEK_SET);
    uint32_t headSize = 0;
    int read = fread(&headSize, sizeof(uint32_t), 1, inputFile);
    readCheck(read, 1, "readDIBHeader", inputFile);
    if(headSize != 0x7C){
        errorOut(noBMPV5, EC_noBMPV5);
    }
    dibHead_t* newDIB = malloc(sizeof(dibHead_t));
    newDIB->headerSize = headSize;
    uint32_t* longBuff = malloc(sizeof(uint32_t) * 14);
    read = fread(longBuff, sizeof(uint32_t), 14, inputFile);
    readCheck(read, 14, "readDIBHeader", inputFile);
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
    read = fread(longBuff, sizeof(uint32_t), 6, inputFile);
    readCheck(read, 6, "readDIBHeader", inputFile);
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
    puts("Created BMP Header");
    newRaw->deviceHeader = readDIBHeader(inputFile);
    puts("Created DIB Header");
    uint32_t colors = newRaw->deviceHeader->colorTableSize;
    printf("Read %d colors...\t\t", colors);
    if(colors){
        uint32_t* cLongs = malloc(sizeof(uint32_t) * colors);
        argbColor_t* colorLUT = malloc(sizeof(argbColor_t) * colors);
        for(uint32_t i = 0; i < colors; i++){
            colorLUT[i] = longToColor(cLongs[i]);
        }
        newRaw->colorTable = colorLUT;
        free(cLongs);
        free(colorLUT);
        puts("Saved");
    } else {
        newRaw->colorTable = NULL;
        puts("Ignored");
    }
    int32_t iW = newRaw->deviceHeader->imageWidth;
    int32_t iH = newRaw->deviceHeader->imageHeight;
    uint32_t picOff = newRaw->fileHeader->offToPixels;
    uint64_t pixCount = iW * iH;
    printf("Width: %" PRId32"\tHeight: %" PRId32 "\tOffset: %" PRId32\
        "\tPixel Count: %" PRIu64 "\n", iW, iH, picOff, pixCount);
    uint32_t* pix = malloc(sizeof(uint32_t) * pixCount);
    uint32_t* rowBuff = malloc(sizeof(uint32_t) * iW);
    uint8_t pixBuff[3] = {0x00, 0x00, 0x00};
    for(int r = (iH - 1); r >= 0; r--){
        if(newRaw->deviceHeader->compression == BI_BITFIELDS){
            int offset = picOff + (r * iW * sizeof(uint32_t));
            fseek(inputFile, offset, SEEK_SET);
            int read = fread(rowBuff, sizeof(uint32_t), iW, inputFile);
            readCheck(read, iW, "createRawBMP", inputFile);
        } else {
            int padCount = (4 - (iW * 3) % 4) % 4;
            for(int c = 0; c < iW; c++){
                int offset = picOff + (r * iW * 3) + (c * 3) + (r * padCount);
                fseek(inputFile, offset, SEEK_SET);
                int read = fread(pixBuff, 1, 3, inputFile);
                readCheck(read, 3, "createRawBMP", inputFile);
                uint32_t fakePixel = 0xFF000000;
                fakePixel += pixBuff[2] << 16;
                fakePixel += pixBuff[1] << 8;
                fakePixel += pixBuff[0];
                rowBuff[c] = fakePixel;
            }
        }
        for(int c = 0; c < iW; c++){
            int pixelOff = ((iH - 1 - r) * iW) + c;
            pix[pixelOff] = rowBuff[c];
        }
    }
    newRaw->pixelArray = pix;
    puts("Pixels Saved");
    if(newRaw->deviceHeader->iccData){
        uint8_t* icc = malloc(newRaw->deviceHeader->iccSize);
        fseek(inputFile, (14 + newRaw->deviceHeader->iccData), SEEK_SET);
        int read = fread(icc, 1, newRaw->deviceHeader->iccSize, inputFile);
        readCheck(read, newRaw->deviceHeader->iccSize, "createRawBMP",
                    inputFile);
        newRaw->iccProfile = icc;
        puts("ICC Data Saved");
    } else {
        newRaw->iccProfile = NULL;
        puts("ICC Data Ignored");
    }
    free(rowBuff);
    puts("In-memory BMP Created\n");
    return newRaw;
}

int checkBMPColorSpace(bmpRawFile_t* rawBMP, uint32_t space){
    if(rawBMP->deviceHeader->colorSpace == space){
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
        case(SPACE_SRGB):
            puts("(sRGB)");
            break;
        case(SPACE_WINDOWS):
            puts("(Windows Colorspace)");
            break;
        case(SPACE_CALIBRATED):
            puts("(Calibrated)");
            break;
        case(SPACE_LINKED):
            puts("(Linked ICC Profile)");
            break;
        case(SPACE_EMBEDED):
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

bmpRawFile_t* genEmptyRawBMP(void){
    ciePoint_t zeroPoint = {0, 0, 0};
    triplePoint_t zeroTriple = {zeroPoint, zeroPoint, zeroPoint};
    bmpRawFile_t* emptyRaw = malloc(sizeof(bmpRawFile_t));
    uint32_t DIBSize = (uint32_t)sizeof(dibHead_t);
    emptyRaw->colorTable = NULL;
    emptyRaw->iccProfile = NULL;
    emptyRaw->fileHeader = malloc(sizeof(bmpHead_t));
    emptyRaw->fileHeader->signature[0] = 'B';
    emptyRaw->fileHeader->signature[1] = 'M';
    emptyRaw->fileHeader->offToPixels = ((uint32_t)sizeof(bmpHead_t) + DIBSize);
    emptyRaw->fileHeader->offToPixels -= 2;
    emptyRaw->deviceHeader = malloc(DIBSize);
    emptyRaw->deviceHeader->headerSize = DIBSize;
    emptyRaw->deviceHeader->planeCount = 1;
    emptyRaw->deviceHeader->bitsPerPixel = 32;
    emptyRaw->deviceHeader->compression = BI_BITFIELDS;
    emptyRaw->deviceHeader->pixPerMeterX = 2835;
    emptyRaw->deviceHeader->pixPerMeterY = 2835;
    emptyRaw->deviceHeader->colorTableSize = 0;
    emptyRaw->deviceHeader->numImportant = 0;
    emptyRaw->deviceHeader->bitmaskRed = 0x00FF0000;
    emptyRaw->deviceHeader->bitmaskGreen = 0x0000FF00;
    emptyRaw->deviceHeader->bitmaskBlue = 0x000000FF;
    emptyRaw->deviceHeader->bitmaskAlpha = 0xFF000000;
    emptyRaw->deviceHeader->colorSpace = SPACE_SRGB;
    emptyRaw->deviceHeader->colorEndpoints = zeroTriple;
    emptyRaw->deviceHeader->gammaRed = 0;
    emptyRaw->deviceHeader->gammaGreen = 0;
    emptyRaw->deviceHeader->gammaBlue = 0;
    emptyRaw->deviceHeader->renderIntent = LCS_GM_GRAPHICS;
    emptyRaw->deviceHeader->iccData = 0;
    emptyRaw->deviceHeader->iccSize = 0;
    emptyRaw->pixelArray = NULL; // Until otherwise...
    return emptyRaw;
}

void saveBMPFile(bmpRawFile_t* bmp, char* name){
    FILE* outFile = fopen(name, "wb");
    if(!outFile){
        printf("Error code: %d\n", errno);
        perror("Bugged");
    }
    // BMP File Header
    puts("Writing file header");
    fwrite(bmp->fileHeader->signature, 1, 2, outFile);
    fwrite(&bmp->fileHeader->fileSize, 4, 1, outFile);
    fwrite(&bmp->fileHeader->reserved1, 2, 1, outFile);
    fwrite(&bmp->fileHeader->reserved2, 2, 1, outFile);
    fwrite(&bmp->fileHeader->offToPixels, 4, 1, outFile);
    // DIB Header
    puts("Writing DIB header");
    fwrite(&bmp->deviceHeader->headerSize, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->imageWidth, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->imageHeight, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->planeCount, 2, 1, outFile);
    fwrite(&bmp->deviceHeader->bitsPerPixel, 2, 1, outFile);
    fwrite(&bmp->deviceHeader->compression, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->imageSize, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->pixPerMeterX, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->pixPerMeterY, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorTableSize, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->numImportant, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->bitmaskRed, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->bitmaskGreen, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->bitmaskBlue, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->bitmaskAlpha, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorSpace, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_red.coord_x, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_red.coord_y, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_red.coord_z, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_green.coord_x, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_green.coord_y, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_green.coord_z, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_blue.coord_x, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_blue.coord_y, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->colorEndpoints.cie_blue.coord_z, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->gammaRed, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->gammaGreen, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->gammaBlue, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->renderIntent, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->iccData, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->iccSize, 4, 1, outFile);
    fwrite(&bmp->deviceHeader->reserved, 4, 1, outFile);
    // Possible Color Table
    if(bmp->colorTable != NULL){
        puts("Writing color table");
        for(uint32_t c = 0; c < bmp->deviceHeader->colorTableSize; c++){
            uint32_t color = colorToLong(bmp->colorTable[c]);
            fwrite(&color, 4, 1, outFile);
        }
    }
    // Pixel Array
    puts("Writing pixel array");
    size_t aiW = abs(bmp->deviceHeader->imageWidth);
    size_t aiH = abs(bmp->deviceHeader->imageHeight);
    size_t size = aiW * aiH;
    printf("Width %zu\tHeight %zu\n", aiW, aiH);
    printf("size: %zu\n", size);
    size *= 4;
    size_t written = fwrite(bmp->pixelArray, 1, size, outFile);
    if(written != size){
        printf("Error: expected %zu, got %zu\n", size, written);
        perror("Bugged");
    }
    // Possible ICC Data
    if(bmp->iccProfile != NULL){
        puts("Writing ICC profile");
        fwrite(bmp->iccProfile, 1, bmp->deviceHeader->iccSize, outFile);
    }
    printf("BMP file \"%s\" written to disk.\n", name);
    fclose(outFile);
    
}
