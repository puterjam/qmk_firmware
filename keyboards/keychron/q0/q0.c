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

#include "q0.h"

#if defined(RGB_MATRIX_ENABLE) && defined(NUM_LOCK_LED_INDEX)

#    define NUM_LOCK_MAX_BRIGHTNESS 0xFF
#    ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#        undef NUM_LOCK_MAX_BRIGHTNESS
#        define NUM_LOCK_MAX_BRIGHTNESS RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    endif

#    define NUM_LOCK_VAL_STEP 8
#    ifdef RGB_MATRIX_VAL_STEP
#        undef NUM_LOCK_VAL_STEP
#        define NUM_LOCK_VAL_STEP RGB_MATRIX_VAL_STEP
#    endif

extern void rgb_matrix_update_pwm_buffers(void);

static uint8_t light_brightness_get(void) {
    uint8_t value = rgb_matrix_get_val();
    if (value < NUM_LOCK_VAL_STEP) {
        value = NUM_LOCK_VAL_STEP;
    } else if (value < (NUM_LOCK_MAX_BRIGHTNESS - NUM_LOCK_VAL_STEP)) {
        value += NUM_LOCK_VAL_STEP; // one step more than current brightness
    } else {
        value = NUM_LOCK_MAX_BRIGHTNESS;
    }

    return value;
}

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
    if (host_keyboard_led_state().num_lock) {
        uint8_t v = light_brightness_get();
        rgb_matrix_set_color(NUM_LOCK_LED_INDEX, v, v, v); // white, with the adjusted brightness
    }
    return true;
}

void rgb_matrix_indicators_none_kb(void) {
    rgb_matrix_indicators_kb();
    rgb_matrix_update_pwm_buffers();
}

bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);

    if (rgb_matrix_is_enabled()
#    if defined(ENABLE_RGB_MATRIX_RAINDROPS)
        && (rgb_matrix_get_mode() != RGB_MATRIX_RAINDROPS)
#    endif
#    if defined(ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS)
        && (rgb_matrix_get_mode() != RGB_MATRIX_JELLYBEAN_RAINDROPS)
#    endif
#    if defined(ENABLE_RGB_MATRIX_PIXEL_RAIN)
        && (rgb_matrix_get_mode() != RGB_MATRIX_PIXEL_RAIN)
#    endif
    ) {
        return res;
    }

    if (res) {
        if (led_state.num_lock) {
            uint8_t v = light_brightness_get();
            rgb_matrix_set_color(NUM_LOCK_LED_INDEX, v, v, v);
        } else {
            rgb_matrix_set_color(NUM_LOCK_LED_INDEX, 0, 0, 0);
        }
        rgb_matrix_update_pwm_buffers();
    }

    return res;
}

#endif
