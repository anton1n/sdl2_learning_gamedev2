cmake_minimum_required(VERSION 3.7)

project(sdl2_learning_gamedev2)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(sdl2_learning_gamedev2 ${SOURCE_FILES})

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} include)
target_link_libraries(sdl2_learning_gamedev2
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        SDL2_image
        SDL2_ttf
)

enable_testing()