cmake_minimum_required(VERSION 3.9)
project(Consommateur C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -ansi -Wall -Wextra -Wconversion -pedantic -O3")

add_executable(Consommateur main.c constantes.h handlers.c handlers.h tools.c tools.h primalite.c primalite.h requete_reponse.h)
target_link_libraries(Consommateur m)