cmake_minimum_required(VERSION 3.0)

project(mhit LANGUAGES C)

set(I_BASE_DIR "./src/incl")

set(I_C_FILES
                  "${I_BASE_DIR}/ansi.c"
                  "${I_BASE_DIR}/bmp.c"
                  "${I_BASE_DIR}/mhps.c"
                  "${I_BASE_DIR}/universal.c"
                  "${I_BASE_DIR}/util.c"
   )

set(I_H_FILES
                  "${I_BASE_DIR}/ansi.h"
                  "${I_BASE_DIR}/bmp.h"
                  "${I_BASE_DIR}/mhps.h"
                  "${I_BASE_DIR}/universal.h"
                  "${I_BASE_DIR}/util.h"
   )

set(INCL_SOURCES ${I_C_FILES} ${I_H_FILES})

add_executable(mhit ./src/main.c ${INCL_SOURCES})

target_include_directories(mhit
                              PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/incl"
                          )

target_link_libraries(mhit m)

install(TARGETS mhit RUNTIME DESTINATION bin)
