/*
 * Copyright 2022 PuterJam (puterjam@gmail.com)
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

#pragma once
#include "color.h"


enum hidrgb_mode {
    HID_MODE_OPENRGB = 1,
    HID_MODE_SIGNALRGB,
};

extern uint16_t g_geekrgb_timer;
extern bool g_geekrgb_anim_playing;

void geekrgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue); //set one led color
void geekrgb_set_mode(int mode); //set hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
uint8_t geekrgb_get_mode(void); //get software mode

void geekrgb_reload_openrgb_colors(void); //reload openrgb colors
void geekrgb_reload_openrgb_anim(void);//reload openrgb colors in animation
RGB* geekrgb_get_openrgb_colors(void); //get openrgb colors
RGB geekrgb_get_openrgb_color(int index);

