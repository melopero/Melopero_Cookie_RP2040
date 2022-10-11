#
#   Melopero Cookie RP2040 Display CircuitPython library
#   Get your Cookie RP2040 here:
#   www.melopero.com/melopero_cookie_rp2040
#
#
#
#   This library is based on the FeatherS2 Neo Helper Library
#   by Seon Rozenblum, Unexpected Maker
#   Project home:
#   https://unexpectedmaker.com/feathers2-neo
#
#
#
#
import time
import neopixel
import board
from micropython import const
from digitalio import DigitalInOut, Direction, Pull
from analogio import AnalogIn

class MeloperoCookieRP2040Display:
    def __init__(self):
		# pin 13 and on-board RGB
        self._led21 = DigitalInOut(board.D21)
        self._led21.direction = Direction.OUTPUT
		

        self._pixel_matrix_power = DigitalInOut(board.D16)
        self._pixel_matrix_power.direction = Direction.OUTPUT



        # Create a NeoPixel matrix reference
        self._matrix = neopixel.NeoPixel(board.D17, 25, brightness=0.5, auto_write=True, pixel_order=neopixel.RGB)

        # Initially set the matrix power off
        self._pixel_matrix_power.value = True


    def set_pixel_matrix_power(self, state):
        """Enable or Disable power to the onboard NeoPixel matrix (D17) and to the onboard Qwiic/StemmaQT connector"""
        self._pixel_matrix_power.value = state



    @staticmethod
    def rgb_color_wheel(wheel_pos):
        """Color wheel to allow for cycling through the rainbow of RGB colors."""
        wheel_pos = wheel_pos % 255

        if wheel_pos < 85:
            return 255 - wheel_pos * 3, 0, wheel_pos * 3
        elif wheel_pos < 170:
            wheel_pos -= 85
            return 0, wheel_pos * 3, 255 - wheel_pos * 3
        else:
            wheel_pos -= 170
            return wheel_pos * 3, 255 - wheel_pos * 3, 0



    @property
    def matrix(self):
        return self._matrix


    @property
    def green_led(self):
        return self._led21.value

    @green_led.setter
    def green_led(self,value):
		# Turn the Blue LED on or off
        self._led21.value = value


class MatrixMessage:

    STATIC = const(0)
    LEFT = const(1)
    RIGHT = const(2)

    def __init__(self, matrix):

        self.matrix = matrix
        self._message_width = 0
        self._message_index = 0
        self._pixel_data = []
        self._scroll_direction = MatrixMessage.LEFT
        self.current_rotation = 0
        self._scroll_delay = 0.15
        self._pixel_data_length = 0
        self._next_tick = 0

        self.glyphs = {
            " ": [0,0,0,0,0],
            "!": [0,29,0,0,0],
            "\"": [0,24,0,24,0],
            "#": [10,31,10,31,10],
            "$": [10,29,21,23,10],
            "%": [25,18,4,9,19],
            "&": [10,21,21,10,1],
            "'": [0,24,0,0,0],
            "(": [0,14,17,0,0],
            ")": [0,17,14,0,0],
            "*": [0,10,4,10,0],
            "+": [0,4,14,4,0],
            ",": [0,1,2,0,0],
            "-": [0,4,4,4,0],
            ".": [0,2,0,0,0],
            "/": [1,2,4,8,16],
            "0": [14,17,17,14,0],
            "1": [0,9,31,1,0],
            "2": [19,21,21,9,0],
            "3": [18,17,21,26,0],
            "4": [6,10,18,31,2],
            "5": [29,21,21,21,18],
            "6": [2,5,13,21,2],
            "7": [17,18,20,24,16],
            "8": [10,21,21,21,10],
            "9": [8,21,22,20,8],
            ":": [0,10,0,0,0],
            ";": [0,1,10,0,0],
            "<": [0,4,10,17,0],
            "=": [0,10,10,10,0],
            ">": [0,17,10,4,0],
            "?": [8,16,21,20,8],
            "@": [14,17,21,18,14],
            "A": [15,20,20,15,0],
            "B": [31,21,21,10,0],
            "C": [14,17,17,17,0],
            "D": [31,17,17,14,0],
            "E": [31,21,21,17,0],
            "F": [31,20,20,16,0],
            "G": [14,17,17,21,6],
            "H": [31,4,4,31,0],
            "I": [17,31,17,0,0],
            "J": [18,17,17,30,16],
            "K": [31,4,10,17,0],
            "L": [31,1,1,1,0],
            "M": [31,8,4,8,31],
            "N": [31,8,4,2,31],
            "O": [14,17,17,14,0],
            "P": [31,20,20,8,0],
            "Q": [12,18,19,13,0],
            "R": [31,20,20,10,1],
            "S": [9,21,21,18,0],
            "T": [16,16,31,16,16],
            "U": [30,1,1,30,0],
            "V": [28,2,1,2,28],
            "W": [31,2,4,2,31],
            "X": [27,4,4,27,0],
            "Y": [16,8,7,8,16],
            "Z": [19,21,25,17,0],
            "[": [0,31,17,17,0],
            "\\": [16,8,4,2,1],
            "]": [0,17,17,31,0],
            "^": [0,8,16,8,0],
            "_": [1,1,1,1,1],
            "`": [0,16,8,0,0],
            "a": [6,9,9,15,1],
            "b": [31,5,5,2,0],
            "c": [6,9,9,9,0],
            "d": [2,5,5,31,0],
            "e": [14,21,21,9,0],
            "f": [4,15,20,16,0],
            "g": [8,21,21,30,0],
            "h": [31,4,4,3,0],
            "i": [0,23,0,0,0],
            "j": [0,1,1,22,0],
            "k": [31,4,10,1,0],
            "l": [0,30,1,1,0],
            "m": [15,8,4,8,15],
            "n": [15,8,8,7,0],
            "o": [6,9,9,6,0],
            "p": [15,10,10,4,0],
            "q": [4,10,10,15,0],
            "r": [7,8,8,8,0],
            "s": [1,5,10,8,0],
            "t": [0,30,5,5,1],
            "u": [14,1,1,15,1],
            "v": [12,2,1,2,12],
            "w": [15,1,2,1,15],
            "x": [9,6,6,9,0],
            "y": [9,5,2,4,8],
            "z": [9,11,13,9,0],
            "{": [0,4,31,17,0],
            "|": [0,31,0,0,0],
            "}": [17,31,4,0,0],
            "~": [0,4,4,2,2],
            '↑': [4,8,31,8,4],
            '→': [4,4,21,14,4],
            '↓': [4,2,31,2,4],
            '←': [4,14,21,4,4],
            '▲': [2, 6, 14, 6, 2],
            '►': [0, 31, 14, 4, 0],
            '▼': [8, 12, 14, 12, 8],
            '◄': [0, 4, 14, 31, 0],
            "☐": [0, 14, 10, 14, 0],
            "□": [31, 17, 17, 17, 31],
            "℃": [24, 24, 7, 5, 5],
            "℉": [24, 24, 7, 6, 4],
            'π': [16, 31, 16, 31, 17],
            'å': [6,9,27,15,1],

        }

        

    def get_characters(self):
        return f"{''.join(sorted(self.glyphs.keys()))} "

    def get_character(self, c):
        if c not in self.glyphs:
            print(f"{c} not in font glyphs, sorry!")
            return None

        glyph_data = self.glyphs[c]
        bits = [0] * 25
        bit = 0
        for y in range(5):
            for x in range(5):
                v = (glyph_data[x] >> (4-y)) & 1
                bits[bit] = v
                bit+=1

        return bits

    def get_message_width(self, txt, use_padding = True):
        total_width = 0
        for i, c in enumerate(txt):
            # Special case for space
            width = 0
            if c == " ":
                width = 2
            elif c in self.glyphs:
                glyph_data = self.glyphs[c]
                for x in range(5):
                    width += 1 if glyph_data[x] > 0 else 0

            # Extra 1 to ensure 1 colum padding for every character in the string
            total_width += (width + 1) if use_padding else width

        return total_width


    def get_message(self, txt, use_padding = True):

        width = self.get_message_width(txt, use_padding)
        # print(f"width: {width}")
        bits = [0] * (width * 5)
        # print(f"len bits {len(bits)}")
        bit = 0
        for i, c in enumerate(txt):
            # Special case for space
            if c == " ":
                bit+= 10
            elif c in self.glyphs:
                glyph_data = self.glyphs[c]
                for x in range(5):
                    if glyph_data[x] > 0:
                        for y in range(5):
                            v = (glyph_data[x] >> (4-y)) & 1
                            bits[bit] = v
                            bit+= 1
            if use_padding:
                bit+= 5

        return width, bits

    def setup_message(self, message, delay=0.2, use_padding=True):
        """ setup the message
            message:        The message to display
            delay:          The scroll speed delay step in ms
            use_padding:    If there should be padding between each character
        """
        self._scroll_delay = delay
        self._message = message
        self._message_width, self._pixel_data = self.get_message(message, use_padding)
        print(self._pixel_data)
        self._pixel_data_length = len(self._pixel_data )
        self._next_tick = time.monotonic()
        self._fade = 1
        # Set the current index to the start or end depending on direction
        self._message_index = self._message_width if self._scroll_direction == MatrixMessage.RIGHT else 0

    def show_message(self, color, brightness = 0.33, fade_out=0.2, restart=False):
        """ show the message on the matrix
            color:      The r,g,b colour each visible LED should be this update
            brightness: Multiplier for the color as the neopixel lib doesn't have a brightness setting
            fade_out:   fade step for each character being show. Only use when display messages in STATIC movement mode
                        to help reduce transition shock and to separate showing identical characters consecutively
        """
        if self._scroll_direction == MatrixMessage.LEFT and self._message_index >= self._message_width:
            if restart:
                self._message_index=0
            return
        elif self._scroll_direction == MatrixMessage.RIGHT and self._message_index <= 0:
            return
        elif self._scroll_direction == MatrixMessage.STATIC and self._message_index >= len(self._message)-1:
            return

        if time.monotonic() > self._next_tick + self._scroll_delay:
            self._next_tick = time.monotonic()
            # Adjust index based on scroll direction
            self._message_index += -1 if self._scroll_direction == MatrixMessage.RIGHT else 1
            self._fade = 1

        if self._scroll_direction == MatrixMessage.STATIC:
            brightness *= self._fade
            self._fade = max(self._fade - fade_out, 0)

        col_on = [c * brightness for c in color ]
        col_off = [0,0,0]

        if self._scroll_direction == MatrixMessage.STATIC:
            for led, p in enumerate(self.get_character(self._message[self._message_index])):
                self.matrix[led] = col_on if p else col_off
            return

        index = 0 + 5 * self._message_index
        
        
        print("outside matrix refresh")
        if index < self._pixel_data_length:
            print("inside matrix refresh")
            for x in range(5):
                for y in range(5):
                    #print(f"x: {x}")
                    #print(f"y: {y}")
                    my_index= ((5*y)+x)
                    
                    if index>=self._pixel_data_length:
                        self.matrix[((5*y)+x)] = col_off
                    else:
                        self.matrix[((5*y)+x)] = col_on if self._pixel_data[index] else col_off
                    index += 1
            


    @property
    def scroll_direction(self):
        return self.scroll_direction

    @scroll_direction.setter
    def scroll_direction(self,value):
		# Set the scroll direction
        self._scroll_direction = value

    @property
    def display_rotation(self):
        return self.current_rotation

    @display_rotation.setter
    def display_rotation(self,value):
		# Set the scroll direction
        self.current_rotation = value

class MatrixAnimation:

    def __init__(self, matrix, anim_type, trail_length):

        # List of animation shapes by pixel index
        # Pixel 0 is Top Left, pixels increase vertically by row
        # Feel free to make your own shapes!
        self.matrix_display_shapes = {
            "square": [0,1,2,3,4,9,14,19,24,23,22,21,20,15,10,5],
            "circle": [1,2,3,9,14,19,23,22,21,15,10,5],
            "diamond": [2,8,14,18,22,16,10,6],
            "plus": [2,7,12,17,22,10,11,12,13,14],
            "cross": [0,6,12,18,24,4,8,12,16,20],
            "spiral": [12,13,18,17,16,11,6,7,8,9,14,19,24,23,22,21,20,15,10,5,0,1,2,3,4,9,14,19,24,23,22,21,20,15,10,5,6,7,8,13,18,17,16,11,12,-1,-1,-1,-1,-1,-1,-1]
        }

        # Initialiation error status
        self.error = False

        if anim_type not in self.matrix_display_shapes:
            print(f"** '{anim_type}' not found in list of shapes!\n** Animation halted!")
            self.error = True
        elif trail_length < 1 or trail_length > 20:
            print(f"** trail_length cannot be {trail_length}. Please pick a value between 1 and 20!\n** Animation halted!")
            self.error = True

        if not self.error:
            self.matrix = matrix
            self.anim_type = anim_type
            self.trail_length = trail_length + 1

            # Create the trail list base don the length of the trail
            self.anim_trail = [x for x in range(0, -self.trail_length,-1)]

            # Create a reference to the selected animation list
            self.current_anim = self.matrix_display_shapes[self.anim_type]

    def get_alpha(self):
        return 0.2 * (self.trail_length-1)

    def inc_anim_index(self, index):
        self.anim_trail[index] += 1
        if self.anim_trail[index] == len(self.current_anim):
            self.anim_trail[index] = 0

    def get_anim_index(self, index ):
        return self.current_anim[self.anim_trail[index]]

    def animate(self, r, g, b):
        if not self.error:
            alpha = self.get_alpha()
            for index in range(self.trail_length):
                if self.anim_trail[index] > -1:
                    (r2, g2, b2) = r * alpha, g * alpha, b * alpha
                    if self.get_anim_index(index) > -1:
                        self.matrix[ self.get_anim_index(index) ] = (r2, g2, b2)
                    alpha = alpha - 0.2 if alpha > 0.2 else 0

                self.inc_anim_index(index)

class MatrixDigitalScope:
    def __init__(self, pin):
        self.pin = pin

    def get_pin(self, col):
        # print(self.pin.value)
        if self.pin.value:
            return 0
        else:
            return 4

# Write your code here :-)
