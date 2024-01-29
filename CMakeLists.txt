cmake_minimum_required(VERSION 3.0)

project(mhit LANGUAGES C)

set(I_BASE_DIR "./src/incl")

set(C_FILES "./src/main.c"
            "${I_BASE_DIR}/universal.c"
            "${I_BASE_DIR}/ansi.c"
            "${I_BASE_DIR}/mhps.c"
            "${I_BASE_DIR}/bmp.c"
            "${I_BASE_DIR}/util.c")

set(H_FILES "${I_BASE_DIR}/universal.h"
            "${I_BASE_DIR}/ansi.h"
            "${I_BASE_DIR}/mhps.h"
            "${I_BASE_DIR}/bmp.h"
            "${I_BASE_DIR}/util.h")

set(ALL_SOURCES ${C_FILES} ${H_FILES})

add_executable(mhit ${ALL_SOURCES})

target_include_directories(mhit PRIVATE
                                "${CMAKE_CURRENT_SOURCE_DIR}/incl"
                          )

install(TARGETS mhit RUNTIME DESTINATION bin)
