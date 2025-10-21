cmake_minimum_required(VERSION 3.9)

# Set C++ standard for all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create folder
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/bin/.placeholder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/bin/.placeholder
    COMMENT "Creating bin folder"
)

add_custom_target(PrepareBinDir ALL DEPENDS ${CMAKE_BINARY_DIR}/bin/.placeholder)

# Copy asset files
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY voxel/maps DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY chip8/roms DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Common settings
add_library(CommonSettings INTERFACE)

target_include_directories(CommonSettings INTERFACE
    rbxe
    3rdparty
)

if(APPLE)
    target_include_directories(CommonSettings INTERFACE /opt/homebrew/Cellar/glfw/3.4/include)
    target_link_libraries(CommonSettings INTERFACE "-framework OpenGL")
    target_link_libraries(CommonSettings INTERFACE "-framework Cocoa")
    target_link_libraries(CommonSettings INTERFACE "-framework IOKit")
    target_link_libraries(CommonSettings INTERFACE /opt/homebrew/Cellar/glfw/3.4/lib/libglfw3.a)
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(CommonSettings INTERFACE glfw m pthread)

    find_package(OpenGL REQUIRED)

    # Link GLFW and GLEW
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    pkg_search_module(GLEW REQUIRED glew)

    target_link_libraries(CommonSettings INTERFACE ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} )
endif()

# RBXE Sources
set(RBXE_SOURCES
    rbxe/rbxe.h
    rbxe/rbxe-arraylist.h
    rbxe/rbxe-font.h
    rbxe/rbxe-gif.h
    rbxe/rbxe-math.h
    rbxe/rbxe-sprite.h
)

# Examples
add_executable(hello examples/hello.c ${RBXE_SOURCES})
target_link_libraries(hello PRIVATE CommonSettings)

add_executable(font examples/font.c ${RBXE_SOURCES})
target_link_libraries(font PRIVATE CommonSettings)

add_executable(sandsim examples/sandsim.c ${RBXE_SOURCES})
target_link_libraries(sandsim PRIVATE CommonSettings)

add_executable(mandelbrot examples/mandelbrot.c ${RBXE_SOURCES})
target_link_libraries(mandelbrot PRIVATE CommonSettings)

add_executable(gameoflife examples/gameoflife.c ${RBXE_SOURCES})
target_link_libraries(gameoflife PRIVATE CommonSettings)

add_executable(gradient examples/gradient.c ${RBXE_SOURCES})
target_link_libraries(gradient PRIVATE CommonSettings)

add_executable(pendulum examples/pendulum.c ${RBXE_SOURCES})
target_link_libraries(pendulum PRIVATE CommonSettings)

add_executable(sprite examples/sprite.c ${RBXE_SOURCES})
target_link_libraries(sprite PRIVATE CommonSettings)

# Game
add_executable(game game/game.c ${RBXE_SOURCES})
target_include_directories(game INTERFACE game)
target_link_libraries(game PRIVATE CommonSettings)

# Voxel
add_executable(voxel voxel/voxel.c ${RBXE_SOURCES})
target_include_directories(voxel INTERFACE voxel)
target_link_libraries(voxel PRIVATE CommonSettings)

# VecChess
add_executable(vec-chess vec-chess/chess-engine.c vec-chess/chess-main.c ${RBXE_SOURCES})
target_include_directories(vec-chess INTERFACE voxel)
target_link_libraries(vec-chess PRIVATE CommonSettings)

# Chip8
add_executable(chip8 chip8/chip8-platform.c chip8/chip8-render.c chip8/chip8-dasm.c chip8/chip8-asm.c chip8/chip8.c chip8/chip8-platform.h chip8/chip8.h ${RBXE_SOURCES})
target_include_directories(chip8 INTERFACE chip8)
target_link_libraries(chip8 PRIVATE CommonSettings)

# GB Emulator
add_executable(gb gameboy/gameboy.c gameboy/platform.c gameboy/gameboy.h ${RBXE_SOURCES})
target_include_directories(gb INTERFACE gb)
target_link_libraries(gb PRIVATE CommonSettings)

install(DIRECTORY ../assets/ DESTINATION ${CMAKE_BINARY_DIR}/bin)