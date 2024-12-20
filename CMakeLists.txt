# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(P2P-Transmitter C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(P2P-Transmitter 
    src/main.c 
    src/packet.c
    src/packet.h
    src/packet-queue.c
    src/packet-queue.h
)

pico_set_program_name(P2P-Transmitter "P2P-Transmitter")
pico_set_program_version(P2P-Transmitter "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(P2P-Transmitter 0)
pico_enable_stdio_usb(P2P-Transmitter 1)
pico_add_extra_outputs(P2P-Transmitter)

# Add the standard library to the build
target_link_libraries(P2P-Transmitter
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(P2P-Transmitter PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(P2P-Transmitter 
        hardware_spi
        hardware_i2c
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_cyw43_arch_none
        )

pico_add_extra_outputs(P2P-Transmitter)

