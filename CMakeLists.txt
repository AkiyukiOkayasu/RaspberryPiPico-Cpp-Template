cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)# Pull in SDK (must be before project)

project(example_project_name C CXX ASM)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C++17
pico_sdk_init()

add_executable(example_project_name
    main.cpp
)

add_compile_options(-Wall
# -Wno-format
# -Wno-unused-function
# -Wno-maybe-uninitialized
)

pico_enable_stdio_usb(example_project_name 1)# Enable stdio to USB serial
pico_enable_stdio_uart(example_project_name 0)# Disable stdio to UART
pico_add_extra_outputs(example_project_name)# Create map/bin/hex/uf2 file etc.
target_link_libraries(example_project_name pico_stdlib)