#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#define STYLE_SRGB          0x73524742  ///< Color space code for "sRGB"
#define STYLE_WINDOWS       0x57696E20  ///< Color space code for "Win "
#define STYLE_CALIBRATED    0x00000000  ///< Color space code for "\0\0\0\0"
#define STYLE_LINKED        0x4C494E4B  ///< Color space code for "LINK"
#define STYLE_EMBEDED       0x4D424544  ///< Color space code for "MBED"

#include "universal.h"

/**
 * @brief A CIE color space endpoint.
 * 
 */
typedef struct ciePoint_t {
    int32_t coord_x;    ///< X coordinate of endpoint.
    int32_t coord_y;    ///< Y coordinate of endpoint.
    int32_t coord_z;    ///< Z coordinate of endpoint.
} ciePoint_t;

/**
 * @brief A collection of CIE points that together define a color space.
 * 
 */
typedef struct triplePoint_t {
    ciePoint_t    cie_red;      ///< Point for the red channel.
    ciePoint_t    cie_green;    ///< Point for the green channel.
    ciePoint_t    cie_blue;     ///< Point for the blue channel.
} triplePoint_t;

/**
 * @brief A struct representation of an ARGB color.
 * 
 */
typedef struct argbColor_t {
    uint8_t alpha;  ///< The color's alpha channel.
    uint8_t red;    ///< The color's red channel.
    uint8_t green;  ///< The color's green channel.
    uint8_t blue;   ///< The color's blue channel.
} argbColor_t;

/**
 * @brief A structure capable of hold a BMP file's header.
 * 
 */
typedef struct bmpHead_t {
    uint8_t     signature[2];   ///< The BMP file's signature.
    uint32_t    fileSize;       ///< The total size of the BMP file in bytes.
    uint16_t    reserved1;      ///< Reserved
    uint16_t    reserved2;      ///< Reserved
    uint32_t    offToPixels;    ///< Offset in bytes to pixel data.
} bmpHead_t;

/**
 * @brief A struct capable of holding a bitmap's DIB header. This specific
 * struct supports the BITMAPV5HEADER format.
 * 
 */
typedef struct dibHead_t { // BITMAPV5HEADER
    uint32_t        headerSize;     ///< Size of DIB header in bytes.
    int32_t         imageWidth;     ///< Width of image in pixels.
    int32_t         imageHeight;    ///< Height of image in pixels.
    uint16_t        planeCount;     ///< Number of bitplanes.
    uint16_t        bitsPerPixel;   ///< Amount of bits per pixel.
    uint32_t        compression;    ///< Compression method used.
    uint32_t        imageSize;      ///< Size of uncompressed image.
    int32_t         pixPerMeterX;   ///< Horizontal pixels per meter.
    int32_t         pixPerMeterY;   ///< Vertical pixels per meter.
    uint32_t        colorTableSize; ///< Size of color lookup table.
    uint32_t        numImportant;   ///< Number of important colors.
    uint32_t        bitmaskRed;     ///< Red channel bitmask.
    uint32_t        bitmaskGreen;   ///< Green channel bitmask.
    uint32_t        bitmaskBlue;    ///< Blue channel bitmask.
    uint32_t        bitmaskAlpha;   ///< Alpha channel bitmask.
    uint32_t        colorSpace;     ///< Colorspace of image
    triplePoint_t   colorEndpoints; ///< Endpoints of calibrated colorspace
    uint32_t        gammaRed;       ///< Red calibrated gamma
    uint32_t        gammaGreen;     ///< Green calibrated gamma
    uint32_t        gammaBlue;      ///< Blue calibrated gamma
    uint32_t        renderIntent;   ///< Image render intent
    uint32_t        iccData;        ///< Offset from start of DIB to ICC Data
    uint32_t        iccSize;        ///< Size of ICC data
    uint32_t        reserved;       ///< Reserved
} dibHead_t;

/**
 * @brief An in-memory representation of a BMP file.
 * 
 */
typedef struct bmpRawFile_t {
    bmpHead_t*      fileHeader;     ///< Base file header
    dibHead_t*      deviceHeader;   ///< Device independant header
    argbColor_t*    colorTable;     ///< Color lookuptable
    uint32_t*       pixelArray;     ///< Pixel array
    uint8_t*        iccProfile;     ///< ICC colorspace data
} bmpRawFile_t;

/**
 * @brief Converts a `long int` to an `argbColor_t` object.
 * 
 * @param value `long int` to covnert.
 * @return `argbColor_t` Resultant color.
 */
argbColor_t longToColor(uint32_t value);

/**
 * @brief Converts an `argbColor_t` object to a `long int`.
 * 
 * @param color Color to convert.
 * @return uint32_t Resultant `long int`.
 */
uint32_t colorToLong(argbColor_t color);

/**
 * @brief Checks if a supplied file stream is of a `.bmp` file. Errors on both a
 * false condition and a malformed header.
 * 
 * @param inputFile File stream to check.
 */
void checkBMPFile(FILE* inputFile);

/**
 * @brief Creates an in-memory copy of a bitmap file's header.
 * 
 * @param inputFile File stream of a `.bmp` file.
 * @return bmpHead_t* Created BMP Header object.
 */
bmpHead_t* readBMPHeader(FILE* inputFile);

/**
 * @brief Reads a set of three CIE endpoints from a file and creates a
 * `triplePoint_t`object from them.
 * 
 * @param inputFile File stream to read from.
 * @return triplePoint_t Resultant set of CIE endpoints.
 */
triplePoint_t readCIETriple(FILE* inputFile);

/**
 * @brief Creates an in-memory copy of a bitmap's DIB header.
 * 
 * @param inputFile File stream of a `.bmp` file.
 * @return dibHead_t* Created DIB Header object.
 */
dibHead_t* readDIBHeader(FILE* inputFile);
/**
 * @brief Create a complete, in-memory representation of a `.bmp` file.
 * 
 * @param inputFile File stream of a `.bmp` file. 
 * @return bmpRawFile_t* Created in-memory representation.
 */
bmpRawFile_t* createRawBMP(FILE* inputFile);

/**
 * @brief Checks if a given BMP is in the supplied colorspace.
 * 
 * @param rawBMP In-memory bitmap to check.
 * @param style Colorspace to check for
 * @return int `1` on success, `0` otherwise.
 */
int checkBMPColorSpace(bmpRawFile_t* rawBMP, uint32_t style);

/**
 * @brief Frees the memory used by an in-memory BMP file.
 * 
 * @param bmp BMP to destroy.
 */
void destroyRawBMP(bmpRawFile_t* bmp);

/**
 * @brief Prints the details of the supplied BMP to the console.
 * 
 * @param bmp BMP to use.
 */
void bmpDetailsOut(bmpRawFile_t* bmp);

#endif // BMP_H_INCLUDED
