cmake_minimum_required(VERSION 3.7)

project(untitled)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} include)

# Add your source files, including Game.cpp
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(untitled ${SOURCE_FILES}
        src/main.cpp)

target_link_libraries(untitled
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        SDL2_image
        SDL2_ttf
)