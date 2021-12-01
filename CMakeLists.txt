cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT TRUE)
include(pico_sdk_import.cmake)# Pull in SDK (must be before project)

project(example_project_name C CXX ASM)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C++17
pico_sdk_init()

# SEGGER RTTのパス変数
set(RTT_DIR ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT_V756d)

add_executable(${PROJECT_NAME}
    main.cpp
    ${RTT_DIR}/RTT/SEGGER_RTT.c
    ${RTT_DIR}/RTT/SEGGER_RTT_printf.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${RTT_DIR}/Config
  ${RTT_DIR}/RTT
  )

add_compile_options(-Wall
# -Wno-format
# -Wno-unused-function
# -Wno-maybe-uninitialized
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)# Enable stdio to USB serial
pico_enable_stdio_uart(${PROJECT_NAME} 0)# Disable stdio to UART
pico_add_extra_outputs(${PROJECT_NAME})# Create map/bin/hex/uf2 file etc.
target_link_libraries(${PROJECT_NAME} pico_stdlib)
