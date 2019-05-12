cmake_minimum_required(VERSION 3.13)
project(Practica_SOA C)

set(CMAKE_C_STANDARD 99)

add_executable(Practica_SOA src/main.c src/argval.c src/argval.h src/types.h src/fat32.c src/fat32.h src/ext4.c src/ext4.h src/identifier.c src/identifier.h src/view.c src/view.h src/date.c src/date.h src/ext4types.h src/fat32types.h)