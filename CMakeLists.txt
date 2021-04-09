cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_USER_MAKE_RULES_OVERRIDE cmake/override.cmake)

set(CMAKE_C_COMPILER "C:/Users/HP/scoop/apps/sdcc/4.1.0/bin/sdcc.exe")

# make CMake happy with sdcc
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE MinSizeRel)

project(firmware
    LANGUAGES C
)

add_executable(${PROJECT_NAME} main.c)

# set(PROG_TOOL "pk3")
set(PROG_TOOL "icd3")
set(MCU_ID "12f617")
set(IPECMD "C:/Program Files (x86)/Microchip/MPLABX/v3.55/mplab_ipe/ipecmd.exe")

set(CMAKE_C_FLAGS "--use-non-free -mpic14 -p${MCU_ID}")

# Enable the target power supply - VDD = 4.75 volts.
# -W4.75

add_custom_target(erase
    COMMAND
    ${IPECMD} -TP${PROG_TOOL} -P${MCU_ID} -E -W4.75
    COMMENT "erase ${MCU_ID} with ${PROG_TOOL}..."
)

add_custom_target(flash
    COMMAND
    ${IPECMD} -TP${PROG_TOOL} -P${MCU_ID} -M -W4.75 -F\"${CMAKE_BINARY_DIR}/firmware.hex\"
    DEPENDS all
    COMMENT "flash ${MCU_ID} with ${PROG_TOOL}..."
)
