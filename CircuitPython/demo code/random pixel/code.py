# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time
import board
import neopixel
from random import randrange



pixel_pin = board.D17


# The number of NeoPixels
num_pixels = 25

# The order of the pixel colors - RGB or GRB. Some NeoPixels have red and green reversed!
# For RGBW NeoPixels, simply change the ORDER to RGBW or GRBW.
ORDER = neopixel.GRB

pixels = neopixel.NeoPixel(
    pixel_pin, num_pixels, brightness=0.2, auto_write=False, pixel_order=ORDER
)


def wheel(pos):
    # Input a value 0 to 255 to get a color value.
    # The colours are a transition r - g - b - back to r.
    if pos < 0 or pos > 255:
        r = g = b = 0
    elif pos < 85:
        r = int(pos * 3)
        g = int(255 - pos * 3)
        b = 0
    elif pos < 170:
        pos -= 85
        r = int(255 - pos * 3)
        g = 0
        b = int(pos * 3)
    else:
        pos -= 170
        r = 0
        g = int(pos * 3)
        b = int(255 - pos * 3)
    return (r, g, b) if ORDER in (neopixel.RGB, neopixel.GRB) else (r, g, b, 0)


def random_pix_cycle(wait):
    
    
    for j in range(255):
        
        pixels.fill(0)
        pixel_index = (j * 256 // num_pixels) + j
        random_index1=randrange(0, 24, 1)
        random_index2=randrange(0, 24, 1)
        random_index3=randrange(0, 24, 1)
        pixels[random_index1] = wheel(pixel_index & 255)
        pixels.show()
        time.sleep(wait)


while True:


    random_pix_cycle(0.3)  # random pixel cycle with 1ms delay per step


