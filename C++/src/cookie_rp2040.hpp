/*
Melopero Cookie RP2040 C++ library. 

Authors: Francesco Marchetti, Luca Davidian

Get your Cookie RP2040 here: 
http://melopero.com/melopero-cookie-rp2040

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/



#ifndef COOKIE_RP2040_H
#define COOKIE_RP2040_H

#include <vector>
#include <string>
#include <pico/time.h>

#define WORDS_PER_CHAR 25
#define CHAR_WIDTH    5

#define CHAR_HEIGHT    5

#define WS2812_PIN_BASE         17
#define COOKIE_DEFAULT_LED_PIN  21


static const uint8_t BUTTON_A = 10;
static const uint8_t BUTTON_B = 11;
static const uint8_t LED_PIN = 21;


bool repeating_timer_callback(repeating_timer_t *rt);

enum class Direction : uint8_t
{
    LEFT, RIGHT, UP, DOWN,
};


class Cookie_RP2040
{
friend bool repeating_timer_callback(struct repeating_timer *t); 
public: 

    uint32_t background_col = 0x00100000;
    uint32_t char_col = 0x10101000;
    
    //constructor
    Cookie_RP2040();

    //destructor
    ~Cookie_RP2040();
    
    //methods
    void show_message(const std::string &message, uint32_t timeMS = 200); 
    void set_rgb_color(uint8_t r, uint8_t g, uint8_t b, double brightness = 0.3);
    void set_rgb_background(uint8_t r, uint8_t g, uint8_t b, double brightness = 0.3);
    void set_direction(Direction direction);
    void set_repeated_start(bool enable);
    void stop_message();
    void clear_screen(uint32_t background = 0);
    void set_pixel(int pix, uint8_t r, uint8_t g, uint8_t b, double brightness = 0.3);
    void set_matrix(uint32_t pixels[25]) ;
    void show_pixels();
    uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, double brightness);


    //Buttons A,B
    void buttons_init();
    bool read_button(uint8_t button);
    
    //Built-in LED
    void led_init();
    void led_on();
    void led_off();
    void led_toggle();


private:
    
    
    static Cookie_RP2040* cookie_rp2040;


    std::vector<uint32_t> mBuffer;
    std::vector<uint32_t>::size_type mDataIndex = 0;
    Direction mDirection = Direction::LEFT;
    bool repeated_start = false;
    bool message_stop = false;
    bool dma_busy = false;
    
    uint32_t mFrame[WORDS_PER_CHAR];
    uint32_t mFrameRow = 0; 
    uint32_t mPixelBuffer[WORDS_PER_CHAR]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint mDMAChannel;
    repeating_timer_t mTimer;
    std::string mMessage;
    uint sm; //state machine 
    uint offset;  //offset of the pio program

    void setup_and_start_timer(uint32_t timeMS);
    
    void pio_init();
    void dma_init();
    
    void remap_buffer();
    std::string const reverse_message(const std::string &message) const;
    



    //todo: change set_pixel(...) name

    
};

#endif  // COOKIE_RP2040_H
