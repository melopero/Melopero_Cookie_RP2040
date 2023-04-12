'''

Melopero Cookie RP2040 example

Get your Cookie RP2040 here:
www.melopero.com/melopero-cookie-rp2040

Authors: Francesco Marchetti, Luca Davidian
Melopero S.r.l.  - www.melopero.com 
First release March 2023

'''


from melopero_cookie import Cookie_RP2040
import time

RIGHT = 1
LEFT = 0
BUTTON_A = 10
BUTTON_B = 11

message = "HELLO COOKIE"

#create a new cookie_rp2040 object
cookie=Cookie_RP2040()


#set neopixel #5 to rgb color (255, 255, 0) with brightness 0.1 (optional)
#if not specified, default brightness is 0.2
cookie.set_pixel(5,255,255,0,0.1)

#set neopixel #15
cookie.set_pixel(15,130,30,0,0.1)

#write the pixels on screen
cookie.show_pixels();

time.sleep(2)

#set all the 25 neopixels at once
#pass as parameters red, green, blue values along with brightness (optional)
#if not specified, default brightness is 0.2
#to write the pixels on screen, call show_pixels()
cookie.set_matrix(20,154,0, 0.1);

#write the pixels on screen
cookie.show_pixels();

time.sleep(2)

#clear the screen by applying a color
#pass as parameters red, green, blue values along with brightness (optional)
#if not specified, default brightness is 0.2
#this fuction write immediately the values on screen, no need to call show_pixels()
cookie.clear_screen(20,0,100, 0.1)

time.sleep(2)

#clear the screen turning off all the leds
cookie.clear_screen()

time.sleep(1)



#set the text color
cookie.set_rgb_color(100,100,0,0.1)


#set_rgb_background()
cookie.set_rgb_background(20,0,100, 0.1)

#enable auto re-start when showing a message
#cookie.enable_repeated_start()

#disable auto re-start when showing a message
#cookie.disable_repeated_start()

#set scrolling direction to RIGHT.
#by default the direction is LEFT
#to change  direction to RIGHT  call set_direction(RIGHT)

#scroll a message on screen
cookie.show_message(message)


#to stop a scrolling text before it has finished,
#you can call stop_message() or clear_message(...)


#Initialize the green LED on GPIO21
cookie.led_init()

#Initialize Buttons A and B respectively on GPIO10 and GPIO11.
cookie.buttons_init()

while True:
    #change current led value to the opposite
    #alternatively led_on() and led_off() can be used
    cookie.led_toggle()
    
    time.sleep(0.2)
    
    #read button A, if true clear the screen with color (20,0,100) and brightness 0.1
    if cookie.read_button(BUTTON_A):   
        cookie.clear_screen(80,30,60, 0.1)
        
    #read button A, if true clear the screen with color (0,0,0) 
    if cookie.read_button(BUTTON_B):
        cookie.clear_screen()
    

