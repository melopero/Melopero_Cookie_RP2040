/*
Melopero Cookie RP2040 Micropython library. 

Authors: Francesco Marchetti, Luca Davidian

Get your Cookie RP2040 here: 
http://melopero.com/melopero-cookie-rp2040

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/


#include "../pico/pico-examples/cookie_rp2040/src/cookie_rp2040.hpp"
#include <cstdio>
#include <string.h>



extern "C" {
    
    #include "cookie_rp2040.h"
    #include "py/builtin.h"
    #include "py/objstr.h"


//variables struct
typedef struct _Cookie_RP2040_obj_t {

    mp_obj_base_t base;
    Cookie_RP2040* cookie;

}_Cookie_RP2040_obj_t;


//Print
void Cookie_RP2040_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind){

    (void)kind; //Unused input parameter
    //_Cookie_RP2040_obj_t *self = MP_OBJ_TO_PTR2(self_in, _Cookie_RP2040_obj_t);
    mp_print_str(print, "Cookie_RP2040()");
}


//Constructor
mp_obj_t Cookie_RP2040_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    
    
    _Cookie_RP2040_obj_t *self = m_new_obj_with_finaliser(_Cookie_RP2040_obj_t);
    Cookie_RP2040 *cookie = new Cookie_RP2040();
    self->base.type = &Cookie_RP2040_type;
    self->cookie = cookie;
    

    
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t Cookie_RP2040___del__(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->cookie->~Cookie_RP2040();

    return mp_const_none;
}


//Methods
mp_obj_t Cookie_RP2040_show_message(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_message, ARG_delay};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_text, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_delay, MP_ARG_INT, {.u_int = 200} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    mp_obj_t message_in = args[ARG_message].u_obj;

    if(!mp_obj_is_str_or_bytes(message_in)) mp_raise_TypeError("message must be a string");
    GET_STR_DATA_LEN(message_in, str, str_len);
    std::string message((const char*)str);
   
    int delay = args[ARG_delay].u_int;
    
    self->cookie->show_message(message, delay);

    return mp_const_none;
}


mp_obj_t Cookie_RP2040_set_pixel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_pix, ARG_r, ARG_g, ARG_b, ARG_brightness};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_pix, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_b, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_brightness, MP_ARG_OBJ, { .u_obj = mp_const_none }},
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    int pix = args[ARG_pix].u_int; 
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;
    
    if (args[ARG_brightness].u_obj != mp_const_none)
     {
        float brightness = mp_obj_get_float(args[ARG_brightness].u_obj);
        self->cookie->set_pixel(pix,r,g,b,brightness);
        return mp_const_none;

     }

    self->cookie->set_pixel(pix,r,g,b);
    return mp_const_none;
}


extern mp_obj_t Cookie_RP2040_show_pixels(mp_obj_t self_in){

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);
    self->cookie->show_pixels();

    return mp_const_none;
}

mp_obj_t Cookie_RP2040_set_matrix(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b, ARG_brightness};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_brightness, MP_ARG_REQUIRED | MP_ARG_OBJ},
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;
    float brightness = mp_obj_get_float(args[ARG_brightness].u_obj);
    
    uint32_t color = self->cookie->urgb_u32(r,g,b,brightness);
    uint32_t pixels[25];
    
    for(int i=0; i<25; i++)
    {
        pixels[i]=color;
    }

    self->cookie->set_matrix(pixels);
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_clear_screen(size_t n_args, const mp_obj_t *args) {

  
    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(args[0]);
    if(n_args>1)
    {
        enum{ARG_self,ARG_r, ARG_g, ARG_b, ARG_brightness};
        uint8_t r = mp_obj_get_int(args[ARG_r]);
        uint8_t g = mp_obj_get_int(args[ARG_g]);
        uint8_t b = mp_obj_get_int(args[ARG_b]);
        float brightness = mp_obj_get_float(args[ARG_brightness]);

        uint32_t color = self->cookie->urgb_u32(r,g,b,brightness);
        self->cookie->clear_screen(color);
        return mp_const_none;
    }

    self->cookie->clear_screen();
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_enable_repeated_start(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->set_repeated_start(true);
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_disable_repeated_start(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->set_repeated_start(false);
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_stop_message(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->stop_message();
    return mp_const_none;
}


mp_obj_t Cookie_RP2040_set_direction(mp_obj_t self_in, mp_obj_t direction) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);
    //uint8_t dir = mp_obj_get_int(direction);
    int dir = mp_obj_get_int(direction);
    if((dir <0) | (dir >1)) 
    {
        return mp_const_none;
    }
    self->cookie->set_direction(static_cast<Direction>(dir));
    return mp_const_none;
}




mp_obj_t Cookie_RP2040_set_rgb_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b, ARG_brightness};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_b, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_brightness, MP_ARG_OBJ, { .u_obj = mp_const_none }},
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;

     if (args[ARG_brightness].u_obj != mp_const_none)
     {
        float brightness = mp_obj_get_float(args[ARG_brightness].u_obj);
        self->cookie->set_rgb_color(r,g,b,brightness);
        return mp_const_none;

     }
    
    self->cookie->set_rgb_color(r,g,b);
    return mp_const_none;
    
}
mp_obj_t Cookie_RP2040_set_rgb_background(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum { ARG_self, ARG_r, ARG_g, ARG_b, ARG_brightness};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_r, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_g, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_b, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_brightness, MP_ARG_OBJ, { .u_obj = mp_const_none }},
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(args[ARG_self].u_obj);

    
    int r = args[ARG_r].u_int;
    int g = args[ARG_g].u_int;
    int b = args[ARG_b].u_int;

     if (args[ARG_brightness].u_obj != mp_const_none)
     {
        float brightness = mp_obj_get_float(args[ARG_brightness].u_obj);
        self->cookie->set_rgb_background(r,g,b,brightness);
        return mp_const_none;

     }
    
    self->cookie->set_rgb_background(r,g,b);
    return mp_const_none;
    
}

mp_obj_t Cookie_RP2040_led_init(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->led_init();
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_led_on(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->led_on();
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_led_off(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);

    self->cookie->led_off();
    return mp_const_none;
}

mp_obj_t Cookie_RP2040_led_toggle(mp_obj_t self_in) {

    _Cookie_RP2040_obj_t *self = (_Cookie_RP2040_obj_t*) MP_OBJ_TO_PTR(self_in);
    
    self->cookie->led_toggle();
    return mp_const_none;
}
    



}//extern C


 

