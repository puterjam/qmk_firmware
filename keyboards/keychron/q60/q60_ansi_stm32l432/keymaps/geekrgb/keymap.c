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

#include "color.h"
#include "keycode.h"
#include "keycode_legacy.h"
#include "quantum_keycodes.h"
#include "usb_descriptor.h"
#include QMK_KEYBOARD_H
#include "geekrgb_common.h"
#include "eeconfig.h"

// clang-format off

enum layers {
    MAC_BASE,
    WIN_BASE,
    MAC_FUNC,
    WIN_FUNC,
    M_PRE, // MAC_BASE preserve layer
    W_PRE, // WIN_BASE preserve layer
};

// Extend custom keycodes from geekrgb_common.h
enum {
    KC_DEBS = KC_GEEKRGB_MODE + 1,  // Del/Backspace swap key
};

// User configuration structure
typedef union {
    uint32_t raw;
    struct {
        bool del_bspc_swapped : 1;  // HHKB-style Del/Backspace swap
        // Reserve remaining bits for future use
        uint32_t reserved : 31;
    };
} user_config_t;

user_config_t user_config;

// Animation state for delete key blink
static uint8_t del_blink_count = 0;      // Number of blinks remaining
static uint32_t del_blink_timer = 0;     // Timer for blink animation
static bool del_blink_state = false;     // Current blink state (on/off)

// Caps Lock state tracking
static bool caps_lock_state = false;

#define BLINK_DURATION 150  // Duration of each blink phase in ms
#define TOTAL_BLINKS 3      // Total number of blinks

// Save user config to EEPROM
void save_user_config(void) {
    eeconfig_update_user(user_config.raw);
}

// Load user config from EEPROM
void load_user_config(void) {
    user_config.raw = eeconfig_read_user();
    // 如果是首次使用（EEPROM 为空），设置默认值
    if (user_config.raw == 0 || user_config.raw == 0xFFFFFFFF) {
        user_config.del_bspc_swapped = true;  // 默认 BS（退格）
        save_user_config();
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_60(
        KC_ESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,    KC_BSLS, KC_GRV,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,            KC_DEBS,
        KC_LCTL, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,                      KC_ENT,
        KC_LSFT,           KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, MO(MAC_FUNC),
                           KC_LOPTN,  KC_LCMMD,                            KC_SPC,                             KC_RCMMD,  KC_GLOBE),

    [WIN_BASE] = LAYOUT_ansi_60(
        KC_ESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,    KC_BSLS, KC_GRV,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,            KC_DEBS,
        KC_LCTL, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,                      KC_ENT,
        KC_LSFT,           KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, MO(WIN_FUNC),
                           KC_LALT,  KC_LWIN,                            KC_SPC,                             KC_RWIN,  KC_RALT),

    [MAC_FUNC] = LAYOUT_ansi_60(
        TG(M_PRE), KC_BRID,  KC_BRIU,  KC_MCTL, KC_SPOT, KC_SIRI, KC_F6,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,  KC_VOLU,    KC_INS, KC_DEL,
        KC_CAPS, RGB_MOD,  RGB_VAD,  RGB_VAI, RGB_SPD, RGB_SPI, _______, _______, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP,  _______,            KC_BSPC,
        _______, KC_VOLD,  KC_VOLU,  KC_MUTE, KC_EJCT, _______, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT,                      _______,
        _______,               RGB_HUD,  RGB_HUI, RGB_SAD, RGB_SAI, _______, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN,             _______, _______,
                           _______,  _______,                            _______,                            KC_STOP,  _______),

    [WIN_FUNC] = LAYOUT_ansi_60(
        TG(W_PRE), KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   KC_INS, KC_DEL,
        KC_CAPS, RGB_MOD,  RGB_VAD,  RGB_VAI, RGB_SPD, RGB_SPI, _______, _______, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP,  _______,            KC_BSPC,
        _______, KC_VOLD,  KC_VOLU,  KC_MUTE, KC_EJCT, _______, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT,                      _______,
        _______,               RGB_HUD,  RGB_HUI, RGB_SAD, RGB_SAI, _______, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN,             _______, _______,
                           _______,  _______,                            _______,                            KC_STOP,  _______),

    [M_PRE] = LAYOUT_ansi_60(
        _______, KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   _______, _______,
        _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,                      _______,
        _______,           _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,
                           _______,  _______,                            _______,                            _______,  _______),

    [W_PRE] = LAYOUT_ansi_60(
        _______, KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,    _______, _______,
        _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,                      _______,
        _______,           _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______,
                           _______,  _______,                            _______,                            _______,  _______)
};


void keyboard_post_init_user(void) {
    // Load user config from EEPROM on startup
    load_user_config();

    // Start wakeup animation on first power-up
    #ifdef WAKEUP_ANIM_ENABLE
    geekrgb_init_wakeup_anim();
    #endif
}

// Called when keyboard wakes from USB suspend
void suspend_wakeup_init_user(void) {
    // Start wakeup animation when waking from sleep
    #ifdef WAKEUP_ANIM_ENABLE
    geekrgb_init_wakeup_anim();
    #endif
}

void housekeeping_task_user(void) {
    housekeeping_task_geekrgb();

    // Update delete key blink animation
    if (del_blink_count > 0) {
        if (timer_elapsed32(del_blink_timer) >= BLINK_DURATION) {
            del_blink_state = !del_blink_state;
            del_blink_count--;
            del_blink_timer = timer_read32();
        }
    }
}

// Monitor system LED state for Caps Lock
bool led_update_user(led_t led_state) {
    caps_lock_state = led_state.caps_lock;
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_geekrgb(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_DEBS:
            // KC_DEBS acts as Del or Backspace based on swap state
            if (record->event.pressed) {
                if (user_config.del_bspc_swapped) {
                    register_code(KC_BSPC);
                } else {
                    register_code(KC_DEL);
                }
            } else {
                if (user_config.del_bspc_swapped) {
                    unregister_code(KC_BSPC);
                } else {
                    unregister_code(KC_DEL);
                }
            }
            return false;

        case KC_B:
            // Fn + Control + B toggles Del/Backspace swap state
            if (record->event.pressed) {
                if ((IS_LAYER_ON(MAC_FUNC) || IS_LAYER_ON(WIN_FUNC)) &&
                    (get_mods() & MOD_MASK_CTRL)) {
                    user_config.del_bspc_swapped = !user_config.del_bspc_swapped;
                    save_user_config();

                    // Trigger delete key blink animation
                    del_blink_count = TOTAL_BLINKS * 2; // *2 for on/off cycles
                    del_blink_timer = timer_read32();
                    del_blink_state = true;

                    return false;
                }
            }
            break;
    }

    return true;
}

#ifdef RAW_ENABLE
bool dip_switch_update_user(uint8_t index, bool active) {
    if (!dip_switch_update_geekrgb(index, active)) {
        return false;
    }
    return true;
}
#endif // RAW_ENABLE

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if(!rgb_matrix_indicators_advanced_geekrgb(led_min, led_max)) {
        return false;
    }

    // Highlight F1-F12 keys (row 0, cols 1-12) based on active layer
    if (IS_LAYER_ON(M_PRE) || IS_LAYER_ON(W_PRE) || IS_LAYER_ON(MAC_FUNC) || IS_LAYER_ON(WIN_FUNC)) {
        // Set color based on active layer
        RGB color = (IS_LAYER_ON(M_PRE) || IS_LAYER_ON(W_PRE)) ? (RGB){200, 0, 0} : (RGB){200, 200, 200};
        for (uint8_t col = 1; col <= 12; col++) {
            uint8_t led_index = g_led_config.matrix_co[0][col];
            if (led_index != NO_LED) {
                rgb_matrix_set_color(led_index, color.r, color.g, color.b);
            }
        }
    }

    // Caps Lock indicator - highlight left Ctrl key (row 2, col 0) when Caps Lock is on
    if (caps_lock_state) {
        uint8_t caps_led_index = g_led_config.matrix_co[1][0];
        if (caps_led_index != NO_LED) {
            rgb_matrix_set_color(caps_led_index, RGB_WHITE);  // White indicator for Caps Lock
        }
    }

    // Delete key blink animation
    if (del_blink_count > 0 && del_blink_state) {
        // Find the LED index for the delete/backspace key (row 1, col 13)
        uint8_t del_led_index = g_led_config.matrix_co[1][13];
        if (del_led_index != NO_LED) {
            if (user_config.del_bspc_swapped) {
                rgb_matrix_set_color(del_led_index, RGB_GREEN);  // Green for Backspace mode
            } else {
                rgb_matrix_set_color(del_led_index, RGB_RED);   // Yellow for Delete mode
            }
        }
    }

    return true;
}
#endif // RGB_MATRIX_ENABLE
