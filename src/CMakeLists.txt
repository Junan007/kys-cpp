cmake_minimum_required(VERSION 2.6)

project(kys)

set(CMAKE_VERBOSE_MAKEFILE on)
# set(CMAKE_CXX_COMPILER $ENV{CXX})

set(CMAKE_CXX_FLAGS "-g -std=c++14 -pthread -DUSE_SDL_MIXER_AUDIO")

file(GLOB SRC_LIST
    ./*.cpp
    ../common/convert.cpp
    ../common/File.cpp
    ../others/Hanz2Piny.cpp
    ../others/zip.c)

set(LINK_LIBRARIES SDL2 SDL2_ttf SDL2_image SDL2_mixer lua opencc sqlite3 fmt)

if (APPLE)
    set(LINK_LIBRARIES ${LINK_LIBRARIES} iconv)
endif ()

add_executable(kys ${SRC_LIST})
target_link_libraries(kys ${LINK_LIBRARIES})
