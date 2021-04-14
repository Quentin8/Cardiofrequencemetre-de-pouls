cmake_minimum_required(VERSION 3.13)
project(ProjetC_cardio C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetC_cardio main.c affichage.c fichiers.c fir.c iir.c integration.c lecture.c mesure.c)
target_link_libraries(ProjetC_cardio m)
