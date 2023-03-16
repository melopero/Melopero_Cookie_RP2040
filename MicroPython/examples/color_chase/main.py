from machine import Pin
import neopixel
import time


pixel_pin = Pin(17, Pin.OUT)


num_pixels = 25

pixels = neopixel.NeoPixel(pixel_pin, num_pixels)


def wheel(pos):
    # Input a value 0 to 255 to get a color value.
    # The colours are a transition r - g - b - back to r.
    if pos < 0 or pos > 255:
        return (0, 0, 0)
    if pos < 85:
        return (255 - pos * 3, pos * 3, 0)
    if pos < 170:
        pos -= 85
        return (0, 255 - pos * 3, pos * 3)
    pos -= 170
    return (pos * 3, 0, 255 - pos * 3)


def color_chase(color, wait):
    for i in range(num_pixels):
        pixels[i] = color
        time.sleep(wait)
        pixels.write()
        time.sleep(0.1)

colors=[(0,100,0), (100,0,0), (0,0,100)]


while True:
    for i in range(0,len(colors)):
        color_chase(colors[i],0.1)
