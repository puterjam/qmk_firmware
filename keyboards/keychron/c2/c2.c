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

#include "c2.h"

const matrix_row_t matrix_mask[] = {
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111101111,
};

#ifdef DIP_SWITCH_ENABLE

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? 2 : 0));
    }
    return true;
}

#endif // DIP_SWITCH_ENABLE

#if defined(RGB_MATRIX_ENABLE) && defined(NUM_LOCK_LED_INDEX) && defined(CAPS_LOCK_LED_INDEX) && defined(MAC_OS_LED_INDEX) && defined(WIN_OS_LED_INDEX)

extern void rgb_matrix_update_pwm_buffers(void);

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
#    if defined(CAPS_LOCK_LED_INDEX)
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, 255, 255, 255);
    } else {
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, 0, 0, 0);
    }
#    endif // CAPS_LOCK_LED_INDEX
#    if defined(NUM_LOCK_LED_INDEX)
    if (host_keyboard_led_state().num_lock) {
        rgb_matrix_set_color(NUM_LOCK_LED_INDEX, 255, 255, 255);
    } else {
        rgb_matrix_set_color(NUM_LOCK_LED_INDEX, 0, 0, 0);
    }
#    endif // NUM_LOCK_LED_INDEX
#    if defined(MAC_OS_LED_INDEX)
    if (default_layer_state == (1 << 0)) {
        rgb_matrix_set_color(MAC_OS_LED_INDEX, 255, 255, 255);
    } else {
        rgb_matrix_set_color(MAC_OS_LED_INDEX, 0, 0, 0);
    }
#    endif // MAC_OS_LED_INDEX
#    if defined(WIN_OS_LED_INDEX)
    if (default_layer_state == (1 << 2)) {
        rgb_matrix_set_color(WIN_OS_LED_INDEX, 255, 255, 255);
    } else {
        rgb_matrix_set_color(WIN_OS_LED_INDEX, 0, 0, 0);
    }
#    endif // WIN_OS_LED_INDEX
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
#    if defined(NUM_LOCK_LED_INDEX)
        if (led_state.num_lock) {
            rgb_matrix_set_color(NUM_LOCK_LED_INDEX, 255, 255, 255);
        } else {
            rgb_matrix_set_color(NUM_LOCK_LED_INDEX, 0, 0, 0);
        }
#    endif // NUM_LOCK_LED_INDEX
#    if defined(CAPS_LOCK_LED_INDEX)
        if (led_state.caps_lock) {
            rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, 255, 255, 255);
        } else {
            rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, 0, 0, 0);
        }
#    endif // CAPS_LOCK_LED_INDEX

        rgb_matrix_update_pwm_buffers();
    }

    return res;
}

#endif // RGB_MATRIX_ENABLE

#if defined(LED_MATRIX_ENABLE) && defined(NUM_LOCK_LED_INDEX) && defined(CAPS_LOCK_LED_INDEX) && defined(MAC_OS_LED_INDEX) && defined(WIN_OS_LED_INDEX)

extern void led_matrix_update_pwm_buffers(void);

bool led_matrix_indicators_kb(void) {
    if (!led_matrix_indicators_user()) {
        return false;
    }
#    if defined(CAPS_LOCK_LED_INDEX)
    if (host_keyboard_led_state().caps_lock) {
        led_matrix_set_value(CAPS_LOCK_LED_INDEX, 255);
    } else {
        led_matrix_set_value(CAPS_LOCK_LED_INDEX, 0);
    }
#    endif // CAPS_LOCK_LED_INDEX
#    if defined(NUM_LOCK_LED_INDEX)
    if (host_keyboard_led_state().num_lock) {
        led_matrix_set_value(NUM_LOCK_LED_INDEX, 255);
    } else {
        led_matrix_set_value(NUM_LOCK_LED_INDEX, 0);
    }
#    endif // NUM_LOCK_LED_INDEX
#    if defined(MAC_OS_LED_INDEX)
    if (default_layer_state == (1 << 0)) {
        led_matrix_set_value(MAC_OS_LED_INDEX, 255);
    } else {
        led_matrix_set_value(MAC_OS_LED_INDEX, 0);
    }
#    endif // MAC_OS_LED_INDEX
#    if defined(WIN_OS_LED_INDEX)
    if (default_layer_state == (1 << 2)) {
        led_matrix_set_value(WIN_OS_LED_INDEX, 255);
    } else {
        led_matrix_set_value(WIN_OS_LED_INDEX, 0);
    }
#    endif // WIN_OS_LED_INDEX
    return true;
}

void led_matrix_indicators_none_kb(void) {
    led_matrix_indicators_kb();
    led_matrix_update_pwm_buffers();
}

bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);

    if (res) {
#    if defined(NUM_LOCK_LED_INDEX)
        if (led_state.num_lock) {
            led_matrix_set_value(NUM_LOCK_LED_INDEX, 255);
        } else {
            led_matrix_set_value(NUM_LOCK_LED_INDEX, 0);
        }
#    endif // NUM_LOCK_LED_INDEX
#    if defined(CAPS_LOCK_LED_INDEX)
        if (led_state.caps_lock) {
            led_matrix_set_value(CAPS_LOCK_LED_INDEX, 255);
        } else {
            led_matrix_set_value(CAPS_LOCK_LED_INDEX, 0);
        }
#    endif // CAPS_LOCK_LED_INDEX

        led_matrix_update_pwm_buffers();
    }

    return res;
}

#endif
