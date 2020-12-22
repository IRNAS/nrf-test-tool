zephyr_include_directories(.)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/i2c_funs.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gpio.c)