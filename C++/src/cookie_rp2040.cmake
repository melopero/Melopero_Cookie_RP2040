add_library(cookie_rp2040 INTERFACE)

pico_generate_pio_header(cookie_rp2040 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_sources(cookie_rp2040 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/cookie_rp2040.cpp
)


target_include_directories(cookie_rp2040 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(cookie_rp2040 INTERFACE pico_stdlib hardware_dma hardware_irq hardware_pio pico_time)
