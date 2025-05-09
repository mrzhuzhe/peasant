cmake_minimum_required(VERSION 3.16)
set(PROJ_NAME stm32-freertos)
set(STM32_CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)

# Please note: When using CMSIS, it is recommended to use the FreeRTOS version supplied in the
# Cube repository because more recent kernels might be incompatible to the CMSIS
# implementation provided by STM
option(USE_CMSIS_RTOS "Use CMSIS RTOS provided by Cube repository" OFF)
option(USE_CMSIS_RTOS_V2 "Use CMSIS RTOS_V2 provided by Cube repository" OFF)
option(USE_CUBE_FREERTOS "Use the FreeRTOS kernel provided by the Cube repository" ON)

if(USE_CUBE_FREERTOS)
    message(STATUS "Using FreeRTOS provided by Cube repository")
else()
    message(STATUS "Using FreeRTOS from kernel repository")
endif()

if(USE_CMSIS_RTOS AND USE_CMSIS_RTOS_V2)
    message(FATAL_ERROR "Can not use USE_CMSIS_RTOS_V2 together with USE_CMSIS_RTOS!")
endif()

if(USE_CMSIS_RTOS)
    message(STATUS "Compiling CMSIS RTOS support")
elseif(USE_CMSIS_RTOS_V2)
    message(STATUS "Compiling CMSIS RTOS V2 support")
endif()

# This must come before the project call!
set(CMAKE_TOOLCHAIN_FILE ${STM32_CMAKE_PATH}/cmake/stm32_gcc.cmake)
project(${PROJ_NAME} CXX C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# Can be used to print out all devices for the H7 or/and the F4 family
# stm32_print_devices_by_family(FAMILY H7)
# stm32_print_devices_by_family(FAMILY F4)
set(SUPPORTED_BOARDS F103C8)
option(FREERTOS_F103C8_EXAMPLE "Compile FreeRTOS example for the F103C8 board" ON)

set(HAL_COMP_LIST RCC GPIO CORTEX)
set(CMSIS_COMP_LIST "")
set(FREERTOS_COMP_LIST "")
set(FREERTOS_NAMESPACE FreeRTOS)

if(USE_CMSIS_RTOS)
    list(APPEND CMSIS_COMP_LIST RTOS)
endif()

if(USE_CMSIS_RTOS_V2)
    list(APPEND CMSIS_COMP_LIST RTOS_V2)
endif()

if(FREERTOS_F103C8_EXAMPLE)
    list(APPEND CMSIS_COMP_LIST STM32F103C8)
    list(APPEND HAL_COMP_LIST STM32F103C8)
    list(APPEND FREERTOS_COMP_LIST ARM_CM3)

    if(USE_CUBE_FREERTOS)
        # The device family needs to be supplied as a component to use the Cube FreeRTOS sources
        list(APPEND FREERTOS_COMP_LIST STM32F1)
        set(FREERTOS_F1_NAMESPACE ${FREERTOS_NAMESPACE}::STM32::F1)
    else()
        set(FREERTOS_F1_NAMESPACE ${FREERTOS_NAMESPACE})
    endif()
endif()

find_package(CMSIS COMPONENTS ${CMSIS_COMP_LIST} REQUIRED)
find_package(HAL COMPONENTS ${HAL_COMP_LIST} REQUIRED)
find_package(FreeRTOS COMPONENTS ${FREERTOS_COMP_LIST} REQUIRED)

set(PROJECT_SOURCES
    main.c
)

# This is required because FreeRTOSConfig.h, stm32hxx_hal_conf.h and main.h
# need to be included
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(FREERTOS_F103C8_EXAMPLE)
    set(TARGET_NAME ${PROJ_NAME}-F103C8)
    add_executable(${TARGET_NAME})

    target_sources(${TARGET_NAME} PRIVATE ${PROJECT_SOURCES})
    target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${FREERTOS_F1_NAMESPACE}::Timers
        ${FREERTOS_F1_NAMESPACE}::Heap::4
        ${FREERTOS_F1_NAMESPACE}::ARM_CM3
        HAL::STM32::F1::RCC
        HAL::STM32::F1::GPIO
        HAL::STM32::F1::CORTEX
        CMSIS::STM32::F103C8
        STM32::NoSys
    )
    if(USE_CMSIS_RTOS)
        target_link_libraries(${TARGET_NAME} PRIVATE
            CMSIS::STM32::F1::RTOS
        )
    endif()
    if(USE_CMSIS_RTOS_V2)
        target_link_libraries(${TARGET_NAME} PRIVATE
            CMSIS::STM32::F1::RTOS_V2
        )
        target_compile_definitions(${TARGET_NAME} PRIVATE
            USE_CMSIS_RTOS_V2
            CMSIS_RTOS_V2_DEVICE_HEADER="stm32f1xx_hal.h"
        )
    endif()

    stm32_print_size_of_target(${TARGET_NAME})
    stm32_generate_binary_file(${TARGET_NAME})    
endif()