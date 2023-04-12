/*
Melopero Cookie RP2040 Micropython library. 

Authors: Francesco Marchetti, Luca Davidian

Get your Cookie RP2040 here: 
http://melopero.com/melopero-cookie-rp2040

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/


//include MicroPython API
#include "py/runtime.h"


//extern of class definition
extern const mp_obj_type_t Cookie_RP2040_type;

//extern of class methods
extern void Cookie_RP2040_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind);
extern  mp_obj_t Cookie_RP2040_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);
extern mp_obj_t Cookie_RP2040___del__(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_show_message(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t Cookie_RP2040_set_pixel(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t Cookie_RP2040_set_matrix(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t Cookie_RP2040_show_pixels(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_clear_screen(size_t n_args, const mp_obj_t *args);
extern mp_obj_t Cookie_RP2040_enable_repeated_start(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_disable_repeated_start(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_stop_message(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_set_direction(mp_obj_t self_in, mp_obj_t direction);
extern mp_obj_t Cookie_RP2040_set_rgb_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t Cookie_RP2040_set_rgb_background(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t Cookie_RP2040_led_init(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_led_on(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_led_off(mp_obj_t self_in);
extern mp_obj_t Cookie_RP2040_led_toggle(mp_obj_t self_in);

