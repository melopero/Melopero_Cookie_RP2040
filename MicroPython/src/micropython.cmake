set(MOD_NAME cookie_rp2040)
string(TOUPPER ${MOD_NAME} MOD_NAME_UPPER)
add_library(usermod_${MOD_NAME} INTERFACE)


target_sources(usermod_${MOD_NAME} INTERFACE
${CMAKE_CURRENT_LIST_DIR}/${MOD_NAME}.c
${CMAKE_CURRENT_LIST_DIR}/${MOD_NAME}.cpp
${CMAKE_CURRENT_LIST_DIR}/../pico/pico-examples/cookie_rp2040/src/cookie_rp2040.cpp
)

pico_generate_pio_header(usermod_${MOD_NAME} 
${CMAKE_CURRENT_LIST_DIR}/../pico/pico-examples/cookie_rp2040/src/ws2812.pio)

target_include_directories(usermod_${MOD_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../pico-sdk
)

target_compile_definitions(usermod_${MOD_NAME} INTERFACE
    MODULE_COOKIE_RP2040_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_${MOD_NAME})

