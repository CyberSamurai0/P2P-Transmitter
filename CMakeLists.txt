cmake_minimum_required(VERSION 3.27)
project(P2P_Transmitter C)

set(CMAKE_C_STANDARD 11)
set(PICO_SDK_PATH ~/pico-sdk)

include(src/pico_sdk_import.make)
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
