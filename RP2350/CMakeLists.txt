# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

# Include Pico SDK import script
include(pico_sdk_import.cmake)

project(2ch_rs485_hat C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
pico_sdk_init()

# Define main executable
add_executable(rs485_hat
    examples/main.c
    lib/Config/DEV_Config.c
    lib/SC16IS752.c
)

# Define Dual-Channel Ping-Pong executable
add_executable(rs485_pingpong
    examples/dual_ping_pong.c
    lib/Config/DEV_Config.c
    lib/SC16IS752.c
)

# Include directories
target_include_directories(rs485_hat PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
target_include_directories(rs485_pingpong PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Link Pico SDK libraries
target_link_libraries(rs485_hat
    pico_stdlib
    hardware_spi
    hardware_gpio
)
target_link_libraries(rs485_pingpong
    pico_stdlib
    hardware_spi
    hardware_gpio
)

# Enable USB serial output for printf, disable UART output
pico_enable_stdio_usb(rs485_hat 1)
pico_enable_stdio_uart(rs485_hat 0)
pico_enable_stdio_usb(rs485_pingpong 1)
pico_enable_stdio_uart(rs485_pingpong 0)

# Generate .uf2 and other output files
pico_add_extra_outputs(rs485_hat)
pico_add_extra_outputs(rs485_pingpong)
