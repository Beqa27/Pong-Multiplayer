cmake_minimum_required(VERSION 3.10)
project(Pong)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2, SDL2_image, SDL2_ttf, SDL2_net using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_NET REQUIRED SDL2_net)

# Your main file(s)
add_executable(Pong 
    src/main.cpp
    src/ball.cpp
    src/paddle.cpp
    src/variables.cpp
    src/multiplayer.cpp
)

# Include directories
target_include_directories(Pong PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_NET_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Pong
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
