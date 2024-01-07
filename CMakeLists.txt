cmake_minimum_required(VERSION 3.0)

project(picanatool LANGUAGES C)

set(I_BASE_DIR "~/projects/picanatool/incl")

set(C_FILES "main.c" "${I_BASE_DIR}/picop.c" "${I_BASE_DIR}/mhpalspr.c" "${I_BASE_DIR}/helptext.c")

set(H_FILES "${I_BASE_DIR}/picop.h" "${I_BASE_DIR}/mhpalspr.h" "${I_BASE_DIR}/helptext.h")

set(ALL_SOURCES ${C_FILES} ${H_FILES})

add_executable(picanatool ${ALL_SOURCES})

target_include_directories(picanatool PRIVATE
                                "${CMAKE_CURRENT_SOURCE_DIR}/incl"
                          )

install(TARGETS picanatool RUNTIME DESTINATION bin)
