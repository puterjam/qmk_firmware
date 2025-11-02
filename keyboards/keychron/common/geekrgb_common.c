/* Copyright 2022 @ Keychron (https://www.keychron.com)
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

#include "geekrgb.h"
#include "rgb_matrix.h"
#include QMK_KEYBOARD_H
#include "geekrgb_common.h"
#include "keychron_factory_test_common.h"

// Wakeup animation state
static bool wakeup_anim_active = false;
static uint32_t wakeup_anim_timer = 0;
static uint8_t wakeup_anim_frame = 0;

#define WAKEUP_ANIM_DURATION 2000  // Total animation duration in ms
#define WAKEUP_FRAME_TIME 20       // Time per frame in ms
#define WAKEUP_TOTAL_FRAMES (WAKEUP_ANIM_DURATION / WAKEUP_FRAME_TIME)

// bool is_siri_active = false;
// uint32_t siri_timer = 0;

key_combination_t key_comb_list[4] = {
    {2, {KC_LWIN, KC_TAB}},
    {2, {KC_LWIN, KC_E}},
    {3, {KC_LSFT, KC_LCMD, KC_4}},
    {2, {KC_LWIN, KC_C}}
};

static uint8_t mac_keycode[4] = { KC_LOPT, KC_ROPT, KC_LCMD, KC_RCMD };

// Initialize wakeup animation
void geekrgb_init_wakeup_anim(void) {
    wakeup_anim_active = true;
    wakeup_anim_timer = timer_read32();
    wakeup_anim_frame = 0;
}

void housekeeping_task_geekrgb(void) {
    housekeeping_task_ft();

    #ifdef WAKEUP_ANIM_ENABLE
    // Update wakeup animation
    if (wakeup_anim_active) {
        if (timer_elapsed32(wakeup_anim_timer) >= WAKEUP_FRAME_TIME) {
            wakeup_anim_frame++;
            wakeup_anim_timer = timer_read32();

            if (wakeup_anim_frame >= WAKEUP_TOTAL_FRAMES) {
                wakeup_anim_active = false;
                wakeup_anim_frame = 0;
            }
        }
    }
    #endif
    // if (is_siri_active) {
    //     if (sync_timer_elapsed32(siri_timer) >= 500) {
    //         unregister_code(KC_LCMD);
    //         unregister_code(KC_SPACE);
    //         is_siri_active = false;
    //     }
    // }
}

bool process_record_geekrgb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_ft(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_MISSION_CONTROL:
            if (record->event.pressed) {
                host_consumer_send(0x29F);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        case KC_LAUNCHPAD:
            if (record->event.pressed) {
                host_consumer_send(0x2A0);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        // case KC_DND:
        //     if (record->event.pressed) {
        //         host_system_send(0x9B);
        //     } else {
        //         host_system_send(0);
        //     }
        //     return false; // Skip all further processing of this key
        case KC_LOPTN:
        case KC_ROPTN:
        case KC_LCMMD:
        case KC_RCMMD:
            if (record->event.pressed) {
                register_code(mac_keycode[keycode - KC_LOPTN]);
            } else {
                unregister_code(mac_keycode[keycode - KC_LOPTN]);
            }
            return false;  // Skip all further processing of this key
        case KC_SPOT:
            // if (record->event.pressed) {
            //     // if (!is_siri_active) {
            //     //     is_siri_active = true;
            //     //     register_code(KC_LCMD);
            //     //     register_code(KC_SPACE);
            //     // }
            //     // siri_timer = sync_timer_read32();
            // } else {
            //     // Do something else when release
            // }
            if (record->event.pressed) {
                host_consumer_send(0x221);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        case KC_TASK:
        case KC_FLXP:
        case KC_SNAP:
        case KC_CRTA:
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    register_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    unregister_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            }
            return false;  // Skip all further processing of this key
        case KC_GLOBE:
            if (record->event.pressed) {
                host_consumer_send(0x29D);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        case KC_SIRI:
            if (record->event.pressed) {
                host_consumer_send(0x0CF);
            } else {
                host_consumer_send(0);
            }
            return false;  // Skip all further processing of this key
        case KC_GEEKRGB_MODE:
            if (record->event.pressed) {
                rgb_matrix_mode(RGB_MATRIX_GEEKRGB);
                geekrgb_reload_openrgb_anim();
            }
            return false; // Skip all further processing of this key
        default:
            return true;  // Process all other keycodes normally
    }
}

#ifdef RAW_ENABLE
bool dip_switch_update_geekrgb(uint8_t index, bool active) {
    if (!dip_switch_update_ft(index, active)) {
        return false;
    }
    return true;
}
#endif // RAW_ENABLE

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_geekrgb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_ft(led_min, led_max)) {
        return false;
    }

#ifdef WAKEUP_ANIM_ENABLE
    // Wakeup animation: from center to both sides with gradient
    if (wakeup_anim_active) {
        uint8_t total_leds = led_max - led_min + 1;
        // Animation progress: 0 to 255
        uint8_t progress = (uint8_t)(255 * wakeup_anim_frame / WAKEUP_TOTAL_FRAMES);

        // Global fade out effect at the end (last 40% of animation)
        uint8_t global_fade = 255;
        if (wakeup_anim_frame > (WAKEUP_TOTAL_FRAMES * 6 / 10)) {
            // Calculate fade out: from 255 to 0 in the last 40% of animation
            uint8_t fade_progress = (wakeup_anim_frame - (WAKEUP_TOTAL_FRAMES * 6 / 10)) * 255 / (WAKEUP_TOTAL_FRAMES * 4 / 10);
            global_fade = 255 - fade_progress;
        }

        for (uint8_t i = led_min; i <= led_max; i++) {
            // Calculate distance from center (0 at center, 255 at edges)
            uint8_t led_index = i - led_min;
            uint8_t center = total_leds / 2;
            uint8_t distance_from_center;

            if (led_index < center) {
                distance_from_center = (center - led_index) * 255 / center;
            } else {
                distance_from_center = (led_index - center) * 255 / (total_leds - center);
            }

            // Wave expands from center: LEDs light up as wave passes
            int16_t wave_position = progress - distance_from_center;

            uint8_t brightness = 0;
            if (wave_position > 0) {
                if (wave_position < 128) {
                    // Fade in
                    brightness = wave_position * 2;
                } else {
                    // Stay bright
                    brightness = 255;
                }
            }

            // Apply global fade out
            brightness = (brightness * global_fade) / 255;

            // Color gradient: Blue to White
            // Blue (0, 0, 255) -> White (255, 255, 255)
            uint8_t color_progress = (progress + distance_from_center / 2) % 255;
            uint8_t r = (brightness * color_progress) / 255;
            uint8_t g = (brightness * color_progress) / 255;
            uint8_t b = brightness;

            rgb_matrix_set_color(i, r, g, b);
        }
        return false;  // Override other effects during wakeup
    }
#endif
    return true;
}
#endif // RGB_MATRIX_ENABLE

#ifdef LED_MATRIX_ENABLE
bool led_matrix_indicators_advanced_geekrgb(uint8_t led_min, uint8_t led_max) {
    if (!led_matrix_indicators_advanced_ft(led_min, led_max)) {
        return false;
    }
    return true;
}
#endif // LED_MATRIX_ENABLE
