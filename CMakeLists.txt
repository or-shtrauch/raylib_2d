cmake_minimum_required(VERSION 3.10)
project(raylib_2d C)

set(CMAKE_BUILD_TYPE debug)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(subprojects/raylib)

file(GLOB
    SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(raylib_2d
    main.c
    ${SRCS}
)

target_include_directories(${PROJECT_NAME}
                            PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} raylib)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Werror
    # -fsanitize=address
    # -fno-omit-frame-pointer
)

# target_link_options(${PROJECT_NAME} PRIVATE
#     -fsanitize=address
# )