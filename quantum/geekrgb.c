/* Copyright 2022 puterjam<puterjam@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"
#include "geekrgb.h"
#include "raw_hid.h"

#if !defined(GEEKRGB_STARTUP_RED)
#    define GEEKRGB_STARTUP_RED 20
#endif

#if !defined(GEEKRGB_STARTUP_GREEN)
#    define GEEKRGB_STARTUP_GREEN 120
#endif

#if !defined(GEEKRGB_STARTUP_BLUE)
#    define GEEKRGB_STARTUP_BLUE 190
#endif


// global
uint16_t        g_geekrgb_timer                       = 0;
bool            g_geekrgb_anim_playing                = false;

// internals
static uint8_t  geekrgb_mode                           = HID_MODE_OPENRGB;
static RGB      last_openrgb_colors[DRIVER_LED_TOTAL] = {[0 ... DRIVER_LED_TOTAL - 1] = {GEEKRGB_STARTUP_GREEN, GEEKRGB_STARTUP_RED, GEEKRGB_STARTUP_BLUE}};


//set one matrix led color
void geekrgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue){
    if (rgb_matrix_get_mode() != RGB_MATRIX_GEEKRGB) { // if matrix mode not signalrgb, do not streaming led.
        return;
    }

    //cache openRGB colors
    if (geekrgb_get_mode() == HID_MODE_OPENRGB) {
        last_openrgb_colors[index].r = red;
        last_openrgb_colors[index].g = green;
        last_openrgb_colors[index].b = blue;
    }else{
#   ifdef GEEKRGB_USE_UNIVERSAL_BRIGHTNESS
        float brightness = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color(index,brightness * red,brightness * green,brightness * blue);
        #else
        rgb_matrix_set_color(index, red , green, blue);
#   endif
    }
}

 //set openrgb colors
void geekrgb_reload_openrgb_colors(void){
    if (geekrgb_get_mode() == HID_MODE_OPENRGB) {
        for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
            geekrgb_set_color(i,last_openrgb_colors[i].r,last_openrgb_colors[i].g,last_openrgb_colors[i].b);
        }
    }
}

void geekrgb_reload_openrgb_anim(void){
    if (!g_geekrgb_anim_playing) g_geekrgb_timer = 0;
    rgb_matrix_set_color_all(0,0,0);
}

 //get openrgb colors
RGB* geekrgb_get_openrgb_colors(void){
    return last_openrgb_colors;
}

RGB geekrgb_get_openrgb_color(int index){
    return last_openrgb_colors[index];
}

//set hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
void geekrgb_set_mode(int mode){
    geekrgb_mode = mode;
}

//get hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
uint8_t geekrgb_get_mode(void){
    return geekrgb_mode;
}

//receive HIDRGB usbhid raw data.
void rgb_hid_receive(uint8_t *data, uint8_t length) {
    bool send = false;

    switch (*data) {
#ifdef OPENRGB_ENABLE
        case 1 ... 9: {
            send = openrgb_command_handler(data, length);
            break;
        }
#endif

#ifdef SIGNALRGB_SUPPORT_ENABLE
        case 0x21 ... 0x28: {
            send = signal_rgb_command_handler(data, length);
            break;
        }
#endif
        default: {
            // id_unhandled
            break;
        }
    }

    if (send) {
        rgb_hid_send(data, length);
    }
}
