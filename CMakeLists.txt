cmake_minimum_required(VERSION 3.15...3.27)
project(P2P_Transmitter C CXX ASM)

set(CMAKE_C_STANDARD 11)

include(lib/pico-sdk/pico_sdk_init.cmake)

include_directories(src)


pico_sdk_init()

add_executable(P2P_Transmitter
        src/main.c
        src/networking.c
        src/networking.h
        src/uart.c
        src/uart.h
        src/bytes.c
        src/bytes.h
)

target_link_libraries(P2P_Transmitter pico_stdlib)

pico_add_extra_outputs(P2P_Transmitter)