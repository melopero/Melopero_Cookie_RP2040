/*
Melopero Cookie RP2040 Micropython library. 

Authors: Francesco Marchetti, Luca Davidian

Get your Cookie RP2040 here: 
http://melopero.com/melopero-cookie-rp2040

Copyright 2023 Melopero S.r.l. - www.melopero.com

First release March 2023

*/



#include "cookie_rp2040.h"


//Methods

MP_DEFINE_CONST_FUN_OBJ_KW(Cookie_RP2040_show_message_obj, 1, Cookie_RP2040_show_message);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040___del___obj, Cookie_RP2040___del__);
MP_DEFINE_CONST_FUN_OBJ_KW(Cookie_RP2040_set_pixel_obj, 1, Cookie_RP2040_set_pixel);
MP_DEFINE_CONST_FUN_OBJ_KW(Cookie_RP2040_set_matrix_obj, 1, Cookie_RP2040_set_matrix);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_show_pixels_obj, Cookie_RP2040_show_pixels);
MP_DEFINE_CONST_FUN_OBJ_VAR(Cookie_RP2040_clear_screen_obj, 1, Cookie_RP2040_clear_screen);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_enable_repeated_start_obj, Cookie_RP2040_enable_repeated_start);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_disable_repeated_start_obj, Cookie_RP2040_disable_repeated_start);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_stop_message_obj, Cookie_RP2040_stop_message);
MP_DEFINE_CONST_FUN_OBJ_2(Cookie_RP2040_set_direction_obj, Cookie_RP2040_set_direction);
MP_DEFINE_CONST_FUN_OBJ_KW(Cookie_RP2040_set_rgb_color_obj, 1, Cookie_RP2040_set_rgb_color);
MP_DEFINE_CONST_FUN_OBJ_KW(Cookie_RP2040_set_rgb_background_obj, 1, Cookie_RP2040_set_rgb_background);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_led_init_obj, Cookie_RP2040_led_init);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_led_on_obj, Cookie_RP2040_led_on);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_led_off_obj, Cookie_RP2040_led_off);
MP_DEFINE_CONST_FUN_OBJ_1(Cookie_RP2040_led_toggle_obj, Cookie_RP2040_led_toggle);



//Bindings of methods

STATIC const mp_rom_map_elem_t Cookie_RP2040_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_show_message), MP_ROM_PTR(&Cookie_RP2040_show_message_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&Cookie_RP2040___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_show_pixels), MP_ROM_PTR(&Cookie_RP2040_show_pixels_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_pixel), MP_ROM_PTR(&Cookie_RP2040_set_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_matrix), MP_ROM_PTR(&Cookie_RP2040_set_matrix_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear_screen), MP_ROM_PTR(&Cookie_RP2040_clear_screen_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable_repeated_start), MP_ROM_PTR(&Cookie_RP2040_enable_repeated_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_disable_repeated_start), MP_ROM_PTR(&Cookie_RP2040_disable_repeated_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop_message), MP_ROM_PTR(&Cookie_RP2040_stop_message_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_direction), MP_ROM_PTR(&Cookie_RP2040_set_direction_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_rgb_color), MP_ROM_PTR(&Cookie_RP2040_set_rgb_color_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_rgb_background), MP_ROM_PTR(&Cookie_RP2040_set_rgb_background_obj) },
    { MP_ROM_QSTR(MP_QSTR_led_init), MP_ROM_PTR(&Cookie_RP2040_led_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_led_on), MP_ROM_PTR(&Cookie_RP2040_led_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_led_off), MP_ROM_PTR(&Cookie_RP2040_led_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_led_toggle), MP_ROM_PTR(&Cookie_RP2040_led_toggle_obj) },
   
};
STATIC MP_DEFINE_CONST_DICT(Cookie_RP2040_locals_dict, Cookie_RP2040_locals_dict_table);

/***** Class Definition *****/
#ifdef MP_DEFINE_CONST_OBJ_TYPE

MP_DEFINE_CONST_OBJ_TYPE(
    Cookie_RP2040_type,
    MP_QSTR_Cookie_RP2040,
    MP_TYPE_FLAG_NONE,
    make_new, Cookie_RP2040_make_new,
    print, Cookie_RP2040_print,
    locals_dict, (mp_obj_dict_t*)&Cookie_RP2040_locals_dict
);
#else

const mp_obj_type_t Cookie_RP2040_type = {
    { &mp_type_type },
    .name = MP_QSTR_Cookie_RP2040,
    .print = Cookie_RP2040_print,
    .make_new = Cookie_RP2040_make_new,
    .locals_dict = (mp_obj_dict_t*)&Cookie_RP2040_locals_dict,
};
#endif

/***** Globals Table *****/
STATIC const mp_map_elem_t melopero_cookie_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_melopero_cookie) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Cookie_RP2040), (mp_obj_t)&Cookie_RP2040_type },
};
STATIC MP_DEFINE_CONST_DICT(mp_module_melopero_cookie_globals, melopero_cookie_globals_table);

/***** Module Definition *****/
const mp_obj_module_t melopero_cookie_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_melopero_cookie_globals,
};

#if MICROPY_VERSION <= 70144
MP_REGISTER_MODULE(MP_QSTR_melopero_cookie, melopero_cookie_user_cmodule, MODULE_COOKIE_RP2040_ENABLED);
#else
MP_REGISTER_MODULE(MP_QSTR_melopero_cookie, melopero_cookie_user_cmodule);
#endif
