#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/cookie_rp2040.hpp"
#include "pico/stdlib.h"


int main(){

    //create new istance of Cookie_RP2040
    Cookie_RP2040 myCookie;

    const std::string message = "HELLO";

    myCookie.set_rgb_color(255,255,0, 0.1);
    myCookie.set_rgb_background(0,0,128, 0.1);
    myCookie.set_repeated_start(true);
    myCookie.set_direction(Direction::LEFT);
    myCookie.show_message(message, 100);
    // sleep_ms(600);
    // myCookie.stop_message();
    // myCookie.clear_screen();
    
    //sleep_ms(200);
    //myCookie.show_message(message2);

    // myCookie.set_pixel(0,20,50,40,0.2);
    // myCookie.set_pixel(4,110,50,40,0.2);
    // myCookie.set_pixel(24,20,10,240,0.2);    
    // myCookie.show_pixel();
    

    // sleep_ms(2000);

    // uint32_t pix[25];
    // for(int i=0; i<25; i++)
    // {
    //     pix[i]=myCookie.urgb_u32(20,50,40,0.2);
    // }

    // myCookie.set_matrix(pix);
    // myCookie.show_pixel();

    // sleep_ms(2000);

    // for(int i=0; i<25; i++)
    // {
    //     pix[i]=myCookie.urgb_u32(20,10,240,0.2);
    // }

    // myCookie.set_matrix(pix);
    // myCookie.show_pixel();


    // sleep_ms(2000);

    // myCookie.set_pixel(0,255,0,0,0.2);
    // myCookie.set_pixel(4,0,255,0,0.2);
    // myCookie.set_pixel(24,0,0,255,0.2);
    
    // myCookie.show_pixel();

    // const uint LED_PIN = 21;
    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
        
    myCookie.led_init();
    
    while(true) {
       
        // gpio_put(LED_PIN, 1);
        // sleep_ms(100);
        // gpio_put(LED_PIN, 0);
        // sleep_ms(100);
        myCookie.led_on();
        sleep_ms(100);
        myCookie.led_off();
        sleep_ms(100);


     
     
    }

    
}

