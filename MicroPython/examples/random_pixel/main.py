from machine import Pin
from neopixel import NeoPixel
import time
from random import randrange

num_pixels = 25
color = (0,100,0)

strip_pin = Pin(17, Pin.OUT)

pixels = NeoPixel(strip_pin, num_pixels)

def random_pix_cycle(wait):
    for j in range(255):
        pixels.fill((0,0,0))
        pixel_index = (j * 256 // num_pixels) + j
        random_index=randrange(0, 24, 1)
        pixels[random_index] = wheel(pixel_index & 255)
        pixels.write()
        time.sleep(wait)
        
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
    return (r, g, b) #if ORDER in (neopixel.RGB, neopixel.GRB) else (r, g, b, 0)

while True:
    random_pix_cycle(0.3)  # random pixel cycle with 1ms delay per step

# Write your code here :-)

