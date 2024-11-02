cmake_minimum_required(VERSION 3.15...3.27)
project(P2P_Transmitter C)

set(CMAKE_C_STANDARD 11)

include(lib/pico-sdk/pico_sdk_init.cmake)
include_directories(src)

add_executable(P2P_Transmitter
        src/main.c
        src/networking.c
        src/networking.h
        src/uart.c
        src/uart.h
        src/bytes.c
        src/bytes.h
)
