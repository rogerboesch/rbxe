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
    # TBD: target_link_libraries(CommonSettings INTERFACE m GL GLU GLEW)
endif()

# Examples
add_executable(bin/hello examples/hello.c)
target_link_libraries(bin/hello PRIVATE CommonSettings)

add_executable(bin/font examples/font.c)
target_link_libraries(bin/font PRIVATE CommonSettings)

add_executable(bin/sandsim examples/sandsim.c)
target_link_libraries(bin/sandsim PRIVATE CommonSettings)

add_executable(bin/mandelbrot examples/mandelbrot.c)
target_link_libraries(bin/mandelbrot PRIVATE CommonSettings)

add_executable(bin/gameoflife examples/gameoflife.c)
target_link_libraries(bin/gameoflife PRIVATE CommonSettings)

add_executable(bin/gradient examples/gradient.c)
target_link_libraries(bin/gradient PRIVATE CommonSettings)

add_executable(bin/pendulum examples/pendulum.c)
target_link_libraries(bin/pendulum PRIVATE CommonSettings)

add_executable(bin/sprite examples/sprite.c)
target_link_libraries(bin/sprite PRIVATE CommonSettings)

# Game
add_executable(bin/game game/game.c)
target_include_directories(bin/game INTERFACE game)
target_link_libraries(bin/game PRIVATE CommonSettings)

# Voxel
add_executable(bin/voxel voxel/voxel.c)
target_include_directories(bin/voxel INTERFACE voxel)
target_link_libraries(bin/voxel PRIVATE CommonSettings)

# Chip8
add_executable(bin/chip8 chip8/chip8-platform.c chip8/chip8-render.c chip8/chip8.c)
target_include_directories(bin/chip8 INTERFACE chip8)
target_link_libraries(bin/chip8 PRIVATE CommonSettings)

install(DIRECTORY ../assets/ DESTINATION ${CMAKE_BINARY_DIR}/bin)