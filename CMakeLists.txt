cmake_minimum_required(VERSION 3.0)

project(mhit LANGUAGES C)

find_package(SDL2 REQUIRED)

set(I_BASE_DIR "./src/incl")

set(I_C_FILES
                  "${I_BASE_DIR}/ansi.c"
                  "${I_BASE_DIR}/bmp.c"
                  "${I_BASE_DIR}/mhps.c"
                  "${I_BASE_DIR}/universal.c"
                  "${I_BASE_DIR}/util.c"
                  "${I_BASE_DIR}/sdl-code/base-sdl.c"
   )

set(I_H_FILES
                  "${I_BASE_DIR}/ansi.h"
                  "${I_BASE_DIR}/bmp.h"
                  "${I_BASE_DIR}/mhps.h"
                  "${I_BASE_DIR}/universal.h"
                  "${I_BASE_DIR}/util.h"
                  "${I_BASE_DIR}/sdl-code/base-sdl.h"
   )

set(SOURCES ${I_C_FILES} ${I_H_FILES})

add_executable(mhit ./src/main.c ${SOURCES})

target_include_directories(mhit
                              PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/incl"
                              PRIVATE ${SDL2_INCLUDE_DIRS}
                          )

target_link_libraries(mhit m ${SDL2_LIBRARIES})

install(TARGETS mhit RUNTIME DESTINATION bin)
