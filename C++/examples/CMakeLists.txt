add_executable(
  example
  example.cpp
)


# Pull in pico libraries
target_link_libraries(
        example
        cookie_rp2040
	      pico_stdlib
        hardware_dma
        hardware_irq
        hardware_pio
        pico_time
        )



# create map/bin/hex file etc.
pico_add_extra_outputs(example)




