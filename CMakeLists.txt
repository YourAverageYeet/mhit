cmake_minimum_required(VERSION 3.0)

project(mhit LANGUAGES C)

find_package(SDL2 REQUIRED)

set(I_BASE_DIR "./src/incl")

set(I_SDL_DIR "./src/incl/sdl-code")

set(I_C_FILES
                  "${I_BASE_DIR}/ansi.c"
                  "${I_BASE_DIR}/bmp.c"
                  "${I_BASE_DIR}/mhps.c"
                  "${I_BASE_DIR}/universal.c"
                  "${I_BASE_DIR}/util.c"
)

set(I_C_FILES_SDL
                  "${I_SDL_DIR}/base-sdl.c"
                  "${I_SDL_DIR}/minif.c"
                  "${I_SDL_DIR}/sdl-test.c"
                  "${I_SDL_DIR}/disp-mode/mhps-disp.c"
                  "${I_SDL_DIR}/gsv-blocks.c"
)

set(I_H_FILES
                  "${I_BASE_DIR}/ansi.h"
                  "${I_BASE_DIR}/bmp.h"
                  "${I_BASE_DIR}/mhps.h"
                  "${I_BASE_DIR}/universal.h"
                  "${I_BASE_DIR}/util.h"
)

set(I_H_FILES_SDL
                  "${I_SDL_DIR}/base-sdl.h"
                  "${I_SDL_DIR}/minif.h"
                  "${I_SDL_DIR}/sdl-test.h"
                  "${I_SDL_DIR}/disp-mode/mhps-disp.h"
                  "${I_SDL_DIR}/gsv-blocks.h"
)

set(I_SOURCES ${I_C_FILES} ${I_H_FILES} ${I_C_FILES_SDL} ${I_H_FILES_SDL})

add_executable(mhit "./src/main.c" ${I_SOURCES})

target_include_directories(mhit
                              PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/incl"
                              PRIVATE ${SDL2_INCLUDE_DIRS}
                          )

target_link_libraries(mhit m ${SDL2_LIBRARIES})

target_compile_options(mhit PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS mhit RUNTIME DESTINATION bin)
