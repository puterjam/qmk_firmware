/* Copyright 2022 puterjam<git@zvecr.com>
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
#include "hidrgb.h"
#include "raw_hid.h"

#if !defined(HIDRGB_OPENRGB_STARTUP_RED)
#    define HIDRGB_OPENRGB_STARTUP_RED 40
#endif

#if !defined(HIDRGB_OPENRGB_STARTUP_GREEN)
#    define HIDRGB_OPENRGB_STARTUP_GREEN 40
#endif

#if !defined(HIDRGB_OPENRGB_STARTUP_BLUE)
#    define HIDRGB_OPENRGB_STARTUP_BLUE 240
#endif

// internals
static uint8_t         hidrgb_mode     = HID_MODE_OPENRGB;
static RGB last_openrgb_colors[DRIVER_LED_TOTAL] = {[0 ... DRIVER_LED_TOTAL - 1] ={HIDRGB_OPENRGB_STARTUP_RED,HIDRGB_OPENRGB_STARTUP_GREEN,HIDRGB_OPENRGB_STARTUP_BLUE}};

//static RGB last_openrgb_colors[DRIVER_LED_TOTAL] = {[0 ... DRIVER_LED_TOTAL - 1] ={0,0,0}};
//static RGB last_signalrgb_colors[DRIVER_LED_TOTAL] = {[0 ... DRIVER_LED_TOTAL - 1] ={0,0,0}};

//set one matrix led color
void hidrgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue){
#ifdef HIDRGB_USE_UNIVERSAL_BRIGHTNESS
    float brightness = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
    rgb_matrix_set_color(index,brightness * red,brightness * green,brightness * blue);
    #else
    rgb_matrix_set_color(index, red , green, blue);
#endif

    //cache openRGB colors
    uint8_t mode = hidrgb_get_mode();
    if (mode == HID_MODE_OPENRGB) {
        last_openrgb_colors[index].r = red;
        last_openrgb_colors[index].g = green;
        last_openrgb_colors[index].b = blue;
    }

}

 //set openrgb colors
void hidrgb_reload_openrgb_colors(void){
    uint8_t mode = hidrgb_get_mode();

    if (mode == HID_MODE_OPENRGB) {
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
            hidrgb_set_color(i,last_openrgb_colors[i].r,last_openrgb_colors[i].g,last_openrgb_colors[i].b);

        }
    }
}

 //get openrgb colors
RGB* hidrgb_get_openrgb_colors(void){
    return last_openrgb_colors;
}

//set hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
void hidrgb_set_mode(int mode){
    hidrgb_mode = mode;
}

//get hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
uint8_t hidrgb_get_mode(void){
    return hidrgb_mode;
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
