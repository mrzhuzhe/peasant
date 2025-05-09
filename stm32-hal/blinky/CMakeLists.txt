cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/stm32_gcc.cmake)

# Configure here which STM32 target(s) to build
option(BLINKY_F1_EXAMPLE "Compile F1 example" ON)

project(stm32-blinky C ASM)
set(MAIN_SOURCE_FILE blinky.c)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)


set(HAL_COMP_LIST RCC GPIO CORTEX)
set(CMSIS_COMP_LIST "")



list(APPEND CMSIS_COMP_LIST STM32F1)
list(APPEND HAL_COMP_LIST STM32F1)

message(STATUS "CMSIS_COMP_LIST ${CMSIS_COMP_LIST}")
message(STATUS "HAL_COMP_LIST ${HAL_COMP_LIST}")

find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)

# Find all device specific drivers:
#find_package(HAL COMPONENTS STM32L0 STM32F1 STM32F4 REQUIRED)
# Find drivers for all families:
#find_package(HAL COMPONENTS RCC GPIO CORTEX REQUIRED)
# Find LL driver:
#find_package(HAL COMPONENTS LL_GPIO REQUIRED)
# Find everything:
#find_package(HAL REQUIRED)

# STM32F4-Discovery

# STM32VL-Discovery
add_executable(stm32-blinky-f1 ${MAIN_SOURCE_FILE} stm32f1xx_hal_conf.h)
target_link_libraries(stm32-blinky-f1
    HAL::STM32::F1::RCC
    HAL::STM32::F1::GPIO
    HAL::STM32::F1::CORTEX
    CMSIS::STM32::F100RB
    STM32::NoSys
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

stm32_print_size_of_target(stm32-blinky-f1)