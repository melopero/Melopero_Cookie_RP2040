/*
Melopero Cookie RP2040 C++ library. 

Authors: Francesco Marchetti, Luca Davidian

Get your Cookie RP2040 here: 
http://melopero.com/melopero-cookie-rp2040

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cookie_rp2040.hpp"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include <pico/time.h>
#include "font.h"
#include <cstring>



Cookie_RP2040* Cookie_RP2040::cookie_rp2040 = nullptr;

Cookie_RP2040::Cookie_RP2040()
{

    pio_init();
    dma_init();
    cookie_rp2040=this;

}

 Cookie_RP2040::~Cookie_RP2040()
 {
        clear_screen();
        pio_sm_set_enabled(pio0, sm, false);
        dma_channel_abort(mDMAChannel);
        dma_channel_unclaim(mDMAChannel);
        pio_sm_unclaim(pio0, sm);
        pio_remove_program(pio0, &ws2812_program, offset);
        pio_clear_instruction_memory(pio0);	
        gpio_deinit(LED_PIN);

        cookie_rp2040=nullptr;
 }

void Cookie_RP2040::pio_init()
{
    
    PIO pio = pio0;
    sm = pio_claim_unused_sm(pio, true);
    offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN_BASE, 800000);//, false);

}



void Cookie_RP2040::dma_init()
{
   
        mDMAChannel = dma_claim_unused_channel(true);
        dma_channel_config channel_config = dma_channel_get_default_config(mDMAChannel);
        channel_config_set_transfer_data_size(&channel_config, DMA_SIZE_32);
        channel_config_set_read_increment(&channel_config, true);
        channel_config_set_write_increment(&channel_config, false);
        channel_config_set_dreq(&channel_config, DREQ_PIO0_TX0);
        
        uint sm = 0;
       
        dma_channel_configure(
        mDMAChannel,
        &channel_config,
        &pio0_hw->txf[sm],
        NULL,
        25,
        false);

}


void Cookie_RP2040::buttons_init()
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);

    

}

bool Cookie_RP2040::read_button(uint8_t button)
{
    return (gpio_get(button));
}

void Cookie_RP2040::led_init()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

}

void Cookie_RP2040::led_on()
{
    gpio_put(LED_PIN, true);
}

void Cookie_RP2040::led_off()
{
    gpio_put(LED_PIN, false);
}

void Cookie_RP2040::led_toggle()
{
    uint32_t mask = (1UL << LED_PIN);
    gpio_xor_mask(mask);
}


void Cookie_RP2040::show_message(const std::string &message, uint32_t timeMS)
    {
        
        while(dma_busy);
        dma_busy=true;

        if(message_stop) message_stop=false;

        mMessage = message;

        mBuffer.clear();
        mBuffer.resize((message.size() + 2) * WORDS_PER_CHAR);

        
 
        switch (mDirection)
        {
            case Direction::UP:  

                for (int i = 0; i < (int) message.size(); i++)
                {
                    const uint8_t (&character)[CHAR_WIDTH] = glyphs[message[i] - 32];
                    //const uint8_t *character = glyphs[message[i] - 32];  

                    for (int row = 0; row < CHAR_HEIGHT; row++)
                        for (int col = 0; col < CHAR_WIDTH; col++)
                        {
                            uint8_t characterCol = character[col]; 
                            uint32_t color = characterCol & 1 << ((CHAR_HEIGHT - 1) - row) ? char_col : background_col;
                            mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                        }      
                }

                mDataIndex = 0;

                break;

            case Direction::DOWN:
            {
                std::string reversedMessage = reverse_message(message);
                
                for (int i = 0; i < (int)reversedMessage.size(); i++)
                {
                    const uint8_t (&character)[CHAR_WIDTH] = glyphs[reversedMessage[i] - 32];

                    for (int row = 0; row < CHAR_HEIGHT; row++)
                        for (int col = 0; col < CHAR_WIDTH; col++)
                        {
                            uint8_t characterCol = character[col]; 
                            uint32_t color = characterCol & 1 << ((CHAR_HEIGHT - 1) - row) ? char_col : background_col;
                            mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                        } 
                }
                
                mDataIndex = (reversedMessage.size() + 1) * WORDS_PER_CHAR;

                break;
            }    
                
            case Direction::LEFT:

                for (int i = 0; i < (int) message.size(); i++)
                {
                    const uint8_t (&character)[CHAR_WIDTH] = glyphs[message[i] - 32];

                    for (int row = 0; row < CHAR_HEIGHT; row++)
                        for (int col = 0; col < CHAR_WIDTH; col++)
                        {
                            uint8_t characterCol = character[row]; 
                            uint32_t color = characterCol & 1 << col ? char_col : background_col;
                            mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                        } 
                }

                mFrameRow = 0;

                break;

            case Direction::RIGHT:
            {
                std::string reversedMessage = reverse_message(message);

                for (int i = 0; i < (int) reversedMessage.size(); i++)
                {
                    const uint8_t (&character)[CHAR_WIDTH] = glyphs[reversedMessage[i] - 32];
                    
                    for (int row = 0; row < CHAR_HEIGHT; row++)
                        for (int col = 0; col < CHAR_WIDTH; col++)
                        {
                            uint8_t characterCol = character[row]; 
                            uint32_t color = characterCol & 1 << col ? char_col : background_col;
                            mBuffer[row * CHAR_WIDTH + col + (i + 1) * WORDS_PER_CHAR] = color;
                        } 
                }

                mFrameRow = (reversedMessage.size() + 1) * CHAR_WIDTH; 

                break;
            }
        }

        // fill first and last empty character
        for (int row = 0; row < CHAR_HEIGHT; row++)
            for (int col = 0; col < CHAR_WIDTH; col++)
            {
                mBuffer[row * CHAR_WIDTH + col] = background_col;
                mBuffer[row * CHAR_WIDTH + col + (mMessage.size() + 1) * WORDS_PER_CHAR] = background_col;
            } 
 
        setup_and_start_timer(timeMS);
    }

void Cookie_RP2040::setup_and_start_timer(uint32_t timeMS)
    {
        add_repeating_timer_ms(timeMS, repeating_timer_callback, this, &mTimer);
    }

uint32_t Cookie_RP2040::urgb_u32(uint8_t r, uint8_t g, uint8_t b, double brightness)
{
    

       r *= brightness;
       g *= brightness;
       b *= brightness;

    return ((uint32_t)g << 24 | (uint32_t)r << 16 | b << 8);
             

}



void Cookie_RP2040::set_rgb_color(uint8_t r, uint8_t g, uint8_t b, double brightness)
{
    if (brightness >1) {brightness=1;}
    if (brightness <0) {brightness =0.1;}
    char_col = urgb_u32(r, g, b, brightness);
    
}

void Cookie_RP2040::set_rgb_background(uint8_t r, uint8_t g, uint8_t b, double brightness)
{   
    if (brightness >1) {brightness=1;}
    if (brightness <0) {brightness =0.1;}
    background_col = urgb_u32(r, g, b, brightness);
}

void Cookie_RP2040::set_direction(Direction direction)
{
    mDirection = direction;
}

void Cookie_RP2040::remap_buffer()
    {
        for (uint row = 0; row < CHAR_HEIGHT; row++)
            for (uint col = 0; col < CHAR_WIDTH; col++)
                mFrame[row * CHAR_WIDTH + col] = mBuffer[(mFrameRow + col) * CHAR_HEIGHT + (CHAR_HEIGHT - 1) - row];
    }
    

std::string const Cookie_RP2040::reverse_message(const std::string &message) const 
    {
        std::string reversed;
        reversed.resize(message.size());
        uint32_t i = 0;
        for (auto rit = message.rbegin(); rit!= message.rend(); ++rit)
            reversed[i++] = *rit;

        return reversed;
    }

void Cookie_RP2040::set_repeated_start(bool enable)
{
    repeated_start=enable;
}

void Cookie_RP2040::stop_message()
{
    message_stop=true;
    
}


void Cookie_RP2040::clear_screen(uint32_t background)
{
    if(!message_stop) stop_message();

    while(dma_busy);
    dma_busy=true;
    
    for (int i = 0; i<25; i++)
    {
            mFrame[i] = background; 
            mPixelBuffer[i]=0;
            
    }
    
    dma_channel_set_read_addr(mDMAChannel,mFrame, true);
    sleep_ms(1);
    dma_busy=false;

}


void Cookie_RP2040::set_pixel(int pix, uint8_t r, uint8_t g, uint8_t b, double brightness)
{   
    if (brightness >1) {brightness=1;}
    if (brightness <0) {brightness =0.1;}
    uint32_t color = urgb_u32(r,g,b,brightness);
    mPixelBuffer[pix]=color;
}

void Cookie_RP2040::set_matrix(uint32_t pixels[25])
{
   
    for(int i=0; i<25; i++)
    {
        mPixelBuffer[i]=pixels[i];

    }
}

void Cookie_RP2040::show_pixels()
{
    while(dma_busy);

    dma_busy=true;
    

    dma_channel_set_read_addr(mDMAChannel,mPixelBuffer, true);
    
    sleep_ms(1);
    
    dma_busy=false;

}


bool repeating_timer_callback(repeating_timer_t *rt)
{   


    switch (((Cookie_RP2040*)(rt->user_data))->mDirection)
    {
        case Direction::UP:

            if (((Cookie_RP2040 *)(rt->user_data))->mDataIndex >= (((Cookie_RP2040 *)(rt->user_data))->mMessage.size() + 1) * WORDS_PER_CHAR)  
            {
                ((Cookie_RP2040 *)(rt->user_data))->mDataIndex = 0;

                //check if repeated start is enables, if not cancel timer
                 if(!((Cookie_RP2040*)(rt->user_data))->repeated_start)
                    {
                        ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;
                        return false;

                    } 
            }

            //check if stop command has been received    
            if(((Cookie_RP2040*)(rt->user_data))->message_stop)
            {
                //disable stop trigger and cancel timer 
                ((Cookie_RP2040*)(rt->user_data))->message_stop=false;
                ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;

                return false;
            }
                
            dma_channel_set_read_addr(((Cookie_RP2040*)(rt->user_data))->mDMAChannel, ((Cookie_RP2040*)(rt->user_data))->mBuffer.data() + ((Cookie_RP2040*)(rt->user_data))->mDataIndex, true);
            
            ((Cookie_RP2040 *)(rt->user_data))->mDataIndex += CHAR_WIDTH;

            break;

        case Direction::DOWN:

            if (((Cookie_RP2040 *)(rt->user_data))->mDataIndex <= 0)
            {
                ((Cookie_RP2040 *)(rt->user_data))->mDataIndex = (((Cookie_RP2040 *)(rt->user_data))->mMessage.size() + 1) * WORDS_PER_CHAR; 

                //check if repeated start is enables, if not cancel timer
                if(!((Cookie_RP2040*)(rt->user_data))->repeated_start)
                {
                    ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;
                    return false;
                } 
            }

            //check if stop command has been received    
            if(((Cookie_RP2040*)(rt->user_data))->message_stop)
            {
                //disable stop trigger and cancel timer 
                ((Cookie_RP2040*)(rt->user_data))->message_stop=false;
                ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;

                return false;
            }

            dma_channel_set_read_addr(((Cookie_RP2040*)(rt->user_data))->mDMAChannel, ((Cookie_RP2040*)(rt->user_data))->mBuffer.data() + ((Cookie_RP2040*)(rt->user_data))->mDataIndex, true);
            
            ((Cookie_RP2040 *)(rt->user_data))->mDataIndex -= CHAR_WIDTH;
            
            break;

        case Direction::LEFT:

            
            if (((Cookie_RP2040*)(rt->user_data))->mFrameRow >= (((Cookie_RP2040*)(rt->user_data))->mMessage.size() + 1) * CHAR_WIDTH)
                {
                    ((Cookie_RP2040*)(rt->user_data))->mFrameRow = 0;

                    //check if repeated start is enables, if not cancel timer
                    if(!((Cookie_RP2040*)(rt->user_data))->repeated_start)
                    {
                        ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;
                        return false;

                    } 
                }

            //check if stop command has been received    
            if(((Cookie_RP2040*)(rt->user_data))->message_stop)
            {
                //disable stop trigger and cancel timer 
                ((Cookie_RP2040*)(rt->user_data))->message_stop=false;
                ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;

                return false;
            }

            ((Cookie_RP2040*)(rt->user_data))->remap_buffer();
            dma_channel_set_read_addr(((Cookie_RP2040*)(rt->user_data))->mDMAChannel, ((Cookie_RP2040*)(rt->user_data))->mFrame, true);
            
            ((Cookie_RP2040*)(rt->user_data))->mFrameRow++;
            
            break;

        case Direction::RIGHT:

            if (((Cookie_RP2040*)(rt->user_data))->mFrameRow <= 0)
            {
                ((Cookie_RP2040*)(rt->user_data))->mFrameRow = (((Cookie_RP2040*)(rt->user_data))->mMessage.size() + 1) * CHAR_WIDTH;

                //check if repeated start is enables, if not cancel timer
                if(!((Cookie_RP2040*)(rt->user_data))->repeated_start)
                {
                    ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;
                    return false;

                } 

            }

            //check if stop command has been received    
            if(((Cookie_RP2040*)(rt->user_data))->message_stop)
            {
                //disable stop trigger and cancel timer 
                ((Cookie_RP2040*)(rt->user_data))->message_stop=false;
                ((Cookie_RP2040*)(rt->user_data))->dma_busy=false;

                return false;
            }  

            ((Cookie_RP2040*)(rt->user_data))->remap_buffer();
            dma_channel_set_read_addr(((Cookie_RP2040*)(rt->user_data))->mDMAChannel, ((Cookie_RP2040*)(rt->user_data))->mFrame, true);
            
            ((Cookie_RP2040*)(rt->user_data))->mFrameRow--;
            
            break;
    }

    return true;
}


