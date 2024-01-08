cmake_minimum_required(VERSION 3.0)

project(mhit LANGUAGES C)

set(I_BASE_DIR "./incl")

set(C_FILES "main.c")

set(H_FILES "${I_BASE_DIR}/universal.h")

set(ALL_SOURCES ${C_FILES} ${H_FILES})

add_executable(mhit ${ALL_SOURCES})

target_include_directories(mhit PRIVATE
                                "${CMAKE_CURRENT_SOURCE_DIR}/incl"
                          )

install(TARGETS mhit RUNTIME DESTINATION bin)
