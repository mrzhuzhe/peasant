# For information about why and how of this file: https://cmake.org/cmake/help/latest/command/find_package.html

### F1 ###
set(BSP_F1_BOARDS 
    STM32F1xx_Nucleo STM32VL-Discovery STM3210C_EVAL STM3210E_EVAL
)
set(BSP_F1_COMPONENTS
    ak4343 cs43l22 hx8347d ili9320 ili9325 lis302dl spfd5408 st7735 stlm75
    stmpe811
)
set(BSP_F1_SOURCES_STM3210C_EVAL accelerometer audio eeprom io lcd sd ts)
set(BSP_F1_SOURCES_STM3210E_EVAL audio lcd nand nor sd serialflash sram tsensor)
set(BSP_F1_DEVICE_STM32VL_Discovery F100RB)
set(BSP_F1_DEVICE_STM3210C_EVAL F107VC)
set(BSP_F1_DEVICE_STM3210E_EVAL F103ZE)


if(NOT BSP_FIND_COMPONENTS)
    set(BSP_FIND_COMPONENTS ${STM32_SUPPORTED_FAMILIES_LONG_NAME})
endif()

list(REMOVE_DUPLICATES BSP_FIND_COMPONENTS)

foreach(COMP ${BSP_FIND_COMPONENTS})
    string(TOLOWER ${COMP} COMP_L)
    string(TOUPPER ${COMP} COMP_U)
    
    string(REGEX MATCH "^STM32([CFGHLMUW]P?[0-9BL])([0-9A-Z][0-9M][A-Z][0-9A-Z])?_?(M0PLUS|M4|M7)?.*$" COMP_U ${COMP_U})
    if(NOT CMAKE_MATCH_1)
        message(FATAL_ERROR "Unknown BSP component: ${COMP}")
    endif()
    
    if(CMAKE_MATCH_3)
        set(CORE ${CMAKE_MATCH_3})
        set(CORE_C "::${CORE}")
        set(CORE_U "_${CORE}")
    else()
        unset(CORE)
        unset(CORE_C)
        unset(CORE_U)
    endif()

    set(FAMILY ${CMAKE_MATCH_1})
    string(TOLOWER ${FAMILY} FAMILY_L)
    
    if((NOT STM32_CUBE_${FAMILY}_PATH) AND (DEFINED ENV{STM32_CUBE_${FAMILY}_PATH))
        set(STM32_CUBE_${FAMILY}_PATH $ENV{STM32_CUBE_${FAMILY}_PATH} CACHE PATH "Path to STM32Cube${FAMILY}")
        message(STATUS "ENV STM32_CUBE_${FAMILY}_PATH specified, using STM32_CUBE_${FAMILY}_PATH: ${STM32_CUBE_${FAMILY}_PATH}")
    endif()

    if(NOT STM32_CUBE_${FAMILY}_PATH)
        set(STM32_CUBE_${FAMILY}_PATH /opt/STM32Cube${FAMILY} CACHE PATH "Path to STM32Cube${FAMILY}")
        message(STATUS "No STM32_CUBE_${FAMILY}_PATH specified using default: ${STM32_CUBE_${FAMILY}_PATH}")
    endif()

    set(BSP_${FAMILY}_PATH "${STM32_CUBE_${FAMILY}_PATH}/Drivers/BSP")
    if(NOT EXISTS ${BSP_${FAMILY}_PATH})
        continue()
    endif()
    
    set(BSP_${FAMILY}_INCLUDE "${BSP_${FAMILY}_PATH}/Components/Common")
    
    add_library(BSP::STM32::${FAMILY}${CORE_C} INTERFACE IMPORTED)
    target_link_libraries(BSP::STM32::${FAMILY}${CORE_C} INTERFACE STM32::${FAMILY}${CORE_C})
    target_include_directories(BSP::STM32::${FAMILY}${CORE_C} INTERFACE "${BSP_${FAMILY}_PATH}/Components/Common")
        
    foreach(BOARD ${BSP_${FAMILY}_BOARDS})
        string(REPLACE "-" "_" BOARD_CANONICAL ${BOARD})
        string(TOLOWER ${BOARD_CANONICAL} BOARD_CANONICAL_L)
        set(BOARD_DEVICE ${BSP_${FAMILY}_DEVICE_${BOARD_CANONICAL}})
        
        stm32_get_cores(DEV_CORES FAMILY ${FAMILY} DEVICE ${BOARD_DEVICE})
        if(CORE AND (NOT ${CORE} IN_LIST DEV_CORES))
            continue()
        endif()

        find_path(BSP_${BOARD_CANONICAL}_PATH
            NAMES ${BOARD_CANONICAL_L}.h
            PATHS "${BSP_${FAMILY}_PATH}/${BOARD}" "${BSP_${FAMILY}_PATH}/${BSP_${FAMILY}_DIR_${BOARD_CANONICAL}}"
            NO_DEFAULT_PATH
        )
        if (NOT EXISTS ${BSP_${BOARD_CANONICAL}_PATH})
            continue()
        endif()
        
        add_library(BSP::STM32::${BOARD_CANONICAL}${CORE_C} INTERFACE IMPORTED)
        target_link_libraries(BSP::STM32::${BOARD_CANONICAL}${CORE_C} INTERFACE BSP::STM32::${FAMILY}${CORE_C} CMSIS::STM32::${FAMILY}${CORE_C})
        target_include_directories(BSP::STM32::${BOARD_CANONICAL}${CORE_C} INTERFACE "${BSP_${BOARD_CANONICAL}_PATH}")
        target_sources(BSP::STM32::${BOARD_CANONICAL}${CORE_C} INTERFACE "${BSP_${BOARD_CANONICAL}_PATH}/${BOARD_CANONICAL_L}.c")
        
        foreach(SRC ${BSP_${FAMILY}_SOURCES_${BOARD_CANONICAL}})
            target_sources(BSP::STM32::${BOARD_CANONICAL}${CORE_C} INTERFACE "${BSP_${BOARD_CANONICAL}_PATH}/${BOARD_CANONICAL_L}_${SRC}.c")
        endforeach()
        if(BSP_${FAMILY}_DEVICE_${BOARD_CANONICAL})
            target_link_libraries(BSP::STM32::${BOARD_CANONICAL}${CORE_C} INTERFACE CMSIS::STM32::${BSP_${FAMILY}_DEVICE_${BOARD_CANONICAL}}${CORE_C})
        endif()
    endforeach()
    
    foreach(BCOMP ${BSP_${FAMILY}_COMPONENTS}) 
        string(TOLOWER ${BCOMP} BCOMP_L)
        string(TOUPPER ${BCOMP} BCOMP_U)
        
        add_library(BSP::STM32::${FAMILY}${CORE_C}::${BCOMP_U} INTERFACE IMPORTED)
        target_link_libraries(BSP::STM32::${FAMILY}${CORE_C}::${BCOMP_U} INTERFACE BSP::STM32::${FAMILY}${CORE_C} CMSIS::STM32::${FAMILY}${CORE_C})
        target_include_directories(BSP::STM32::${FAMILY}${CORE_C}::${BCOMP_U} INTERFACE "${BSP_${FAMILY}_PATH}/Components/${BCOMP}")
        
        find_file(BSP_${BOARD_CANONICAL}_${BCOMP}_SOURCE
            NAMES ${BCOMP}.c
            PATHS "${BSP_${FAMILY}_PATH}/Components/${BCOMP}"
            NO_DEFAULT_PATH
        )
        if (BSP_${BOARD_CANONICAL}_${BCOMP}_SOURCE)
            target_sources(BSP::STM32::${FAMILY}${CORE_C}::${BCOMP_U} INTERFACE "${BSP_${BOARD_CANONICAL}_${BCOMP}_SOURCE}")
        endif()
    endforeach()
    
    set(BSP_${COMP}_FOUND TRUE)
    
    if(BSP_${COMP}_FOUND)
         list(APPEND BSP_INCLUDE_DIRS "${BSP_${FAMILY}_INCLUDE}")
    endif()
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BSP
    REQUIRED_VARS BSP_INCLUDE_DIRS
    FOUND_VAR BSP_FOUND
    HANDLE_COMPONENTS
)
