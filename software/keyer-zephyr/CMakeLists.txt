
cmake_minimum_required(VERSION 4.2.2)

set(BOARD rpi_pico)

find_package(Zephyr)
project(Keyer)

find_program(PICOTOOL_EXE NAMES picotool REQUIRED)

add_custom_target(flash_picotool
        COMMAND ${PICOTOOL_EXE} load -x ${CMAKE_BINARY_DIR}/zephyr/zephyr.uf2
        COMMENT "Wgrywanie oprogramowania na RP2040 przez picotool..."
        DEPENDS zephyr_final
)

add_subdirectory(src)