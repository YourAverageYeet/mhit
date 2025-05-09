cmake_minimum_required(VERSION 3.10)

project(mhit LANGUAGES C)

find_package(SDL2 CONFIG REQUIRED)

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

add_executable(mhit)

set_property(TARGET mhit PROPERTY C_STANDARD 99)

target_sources(mhit
                    PRIVATE
                            "./src/main.c"
                            ${I_C_FILES}
                            ${I_C_FILES_SDL}
)

target_include_directories(mhit
                                PRIVATE
                                        "${CMAKE_CURRENT_SOURCE_DIR}/src/incl"
)

target_link_libraries(mhit m SDL2::SDL2)

target_compile_options(mhit
                            PRIVATE
                                    -Wall
                                    -Wextra
                                    -Wpedantic
)

install(TARGETS mhit RUNTIME DESTINATION bin)
