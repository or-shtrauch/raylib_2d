cmake_minimum_required(VERSION 3.10)
project(raylib_2d C)

add_subdirectory(subprojects/raylib)

file(GLOB SRCS src/*.c)

# Add source files
add_executable(raylib_2d
    main.c
    ${SRCS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} raylib)
