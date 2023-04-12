#
#   Melopero Cookie RP2040 Display CircuitPython
#	Scrolling text example
#   Get your Cookie RP2040 here:
#   www.melopero.com/melopero_cookie_rp2040
#
#   This  script is based on the FeatherS2 Neo demo code 
#   by Seon Rozenblum, Unexpected Maker
#   Project home:
#   https://unexpectedmaker.com/feathers2-neo


import time, gc
from melopero_cookie_rp2040_display import MeloperoCookieRP2040Display, MatrixMessage, MatrixAnimation

display = MeloperoCookieRP2040Display()
# Turn on the power to the NeoPixel matrix (active_LOW, set to True to disable power)
display.set_pixel_matrix_power(False)

# Create a color wheel index and a
color_index = 0
# color time delay step
NEXT_COL = 0.01

# Initialise the matrix in Message mode, so you can display scrolling text on it
matrix = MatrixMessage(display.matrix)


message = "HELLO COOKIE "



# Set the scroll direction for the message
# valid directions are: direction LEFT, STATIC, RIGHT,
matrix.scroll_direction = matrix.LEFT

# Set the display rotation
# this feature is not supported yet
matrix.display_rotation = 0

# Setup the message, passing it the message, the scroll delay step in ms (default is 0.2) and if there should be padding between each character, or if they should butt up against eachother (default is True)
matrix.setup_message(message, delay=0.1, use_padding=True)

# declare the R G B colors
r,g,b = 0,0,0

message_sent=False

# Rainbow colors on the NeoPixel
while True:
    # Update the color from the color wheel every 10ms
    if time.monotonic() > NEXT_COL + 0.01:
        color_index += 1
        # Get the R,G,B values of the next color
        r,g,b = display.rgb_color_wheel( color_index )

        NEXT_COL = time.monotonic()


        # If the color_index is divisible by 100, flip the state of the green LED on D21
        if color_index % 100 == 0:
            display.green_led = not display.green_led

    # Show the message on the matrix once
    #matrix.show_message(color=[r,g,b], brightness=0.2, fade_out=0.2, restart=False)
    
    # Show the message on the matrix with auto-restart
    matrix.show_message(color=[r,g,b], brightness=0.2, fade_out=0.2, restart=True)