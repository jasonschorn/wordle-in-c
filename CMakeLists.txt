cmake_minimum_required(VERSION 4.0)
project(wordle-in-c C)
set(CMAKE_C_STANDARD 17)
#
# --> Get Raylib
#
include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
)
FetchContent_MakeAvailable(raylib)
#
# --> Files for executable
#
add_executable(wordle-in-c
        main.c
        src/display_manager.c
        src/collision_manager.c
        src/game_loop.c
        src/nav_button_manager.c
        src/text.c
        src/word_manager.c
        src/wordle.c
        include/wordle.h
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/build")

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
