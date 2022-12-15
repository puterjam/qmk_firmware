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
enum hidrgb_mode {
    HID_MODE_OPENRGB = 1,
    HID_MODE_SIGNALRGB,
};

extern RGB g_hidrgb_colors[DRIVER_LED_TOTAL];

void hidrgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue);
void hidrgb_set_mode(int mode);
uint8_t hidrgb_get_mode(void);
