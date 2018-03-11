cmake_minimum_required(VERSION 3.9)
project(Mini_Projet_2 C)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -ansi -Wall -Wextra -Wconversion -Werror -pedantic -O3")

add_executable(Mini_Projet_2 producteur.c tools_error.c tools_error.h ncurses.c ncurses.h gestionnaires.c gestionnaires.h)
target_link_libraries(Mini_Projet_2 ${CURSES_LIBRARIES})