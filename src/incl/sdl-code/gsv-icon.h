/* GIMP RGBA C-Source image dump (gsv-fullres.c) */

#include "gsv-blocks.h"

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char* pixel_data_blocks[5];
  unsigned int   block_sizes[5];
  unsigned int   block_offsets[5];
} gsv_icon = {64, 64, 4, {gsv_block1, gsv_block2, gsv_block3, gsv_block4,
                          gsv_block5},
                         {3640, 3249, 3246, 2985, 3264},
                         {0, 3640, 6889, 10135, 13120}};

