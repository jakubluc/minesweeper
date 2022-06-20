cmake_minimum_required(VERSION 3.12)
project(Minesweeper CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(include)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC MinesweeperCore)

target_include_directories(main PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/include"
			  "${PROJECT_SOURCE_DIR}/src"
                          )
