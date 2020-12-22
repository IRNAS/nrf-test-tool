# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})
project(shell_module)

zephyr_compile_options(-fdiagnostics-color=always)

target_sources(app PRIVATE src/main.c src/communication.c)

add_subdirectory(src/peripherals)
add_subdirectory(src/drivers)
