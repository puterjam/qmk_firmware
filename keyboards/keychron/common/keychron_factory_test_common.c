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

#include QMK_KEYBOARD_H
#include "keycode.h"
#include "quantum_keycodes.h"
#include "via.h"
#include "raw_hid.h"

#define KEY_PRESS_FN     (0x1<<0)
#define KEY_PRESS_STEP_1 (0x1<<1)
#define KEY_PRESS_STEP_2 (0x1<<2)
#define KEY_PRESS_STEP_3 (0x1<<3)
#define KEY_PRESS_STEP_4 (0x1<<4)
#define KEY_PRESS_FACTORY_RESET (KEY_PRESS_FN | KEY_PRESS_STEP_1 | KEY_PRESS_STEP_2)
#define KEY_PRESS_LED_TEST (KEY_PRESS_FN | KEY_PRESS_STEP_3 | KEY_PRESS_STEP_4)

// clang-format off

#if defined(KEYBOARD_keychron_q0_q0_stm32l432)
#    define FN1 1
#    define KC_STEP_1 RGB_SAD // 1
#    define KC_STEP_2 RGB_HUD // 9
#    define KC_STEP_3 KC_MPLY // 3
#    define KC_STEP_4 KC_DEL  // -
#elif (defined(KEYBOARD_keychron_q2_q2_ansi_stm32l432)      || \
    defined(KEYBOARD_keychron_q2_q2_ansi_stm32l432_ec11)    || \
    defined(KEYBOARD_keychron_q2_q2_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_q2_q2_iso_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_q2_q2_jis_stm32l432)          || \
    defined(KEYBOARD_keychron_q2_q2_jis_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_v2_v2_ansi_stm32l432)         || \
    defined(KEYBOARD_keychron_v2_v2_ansi_stm32l432_ec11)    || \
    defined(KEYBOARD_keychron_v2_v2_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_v2_v2_iso_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_v2_v2_jis_stm32l432)          || \
    defined(KEYBOARD_keychron_v2_v2_jis_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_q4_q4_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_q4_q4_rev1_ansi_stm32l432)    || \
    defined(KEYBOARD_keychron_q4_q4_rev2_ansi_stm32l432)    || \
    defined(KEYBOARD_keychron_v4_v4_ansi_stm32l432)         || \
    defined(KEYBOARD_keychron_v4_v4_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_q7_q7_ansi_stm32l432)         || \
    defined(KEYBOARD_keychron_q7_q7_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_v7_v7_ansi_stm32l432)         || \
    defined(KEYBOARD_keychron_v7_v7_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_q8_q8_ansi_stm32l432)         || \
    defined(KEYBOARD_keychron_q8_q8_ansi_stm32l432_ec11)    || \
    defined(KEYBOARD_keychron_q8_q8_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_q8_q8_iso_stm32l432_ec11))    || \
    defined(KEYBOARD_keychron_v8_v8_ansi_stm32l432)         || \
    defined(KEYBOARD_keychron_v8_v8_ansi_stm32l432_ec11)    || \
    defined(KEYBOARD_keychron_v8_v8_iso_stm32l432)          || \
    defined(KEYBOARD_keychron_v8_v8_iso_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_q65_q65_ansi_stm32l432_ec11)
#    define FN1 2
#    define FN2 3
#    define KC_STEP_1 KC_J
#    define KC_STEP_2 KC_Z
#    define KC_STEP_3 KC_RIGHT
#    define KC_STEP_4 KC_HOME
#elif (defined(KEYBOARD_keychron_q9_q9_ansi_stm32l432)       || \
    defined(KEYBOARD_keychron_q9_q9_ansi_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_q9_q9_iso_stm32l432)           || \
    defined(KEYBOARD_keychron_q9_q9_iso_stm32l432_ec11))
#    define FN1 2
#    define FN2 3
#    define KC_STEP_1 KC_J
#    define KC_STEP_2 RGB_RMOD
#    define KC_STEP_3 KC_RIGHT
#    define KC_STEP_4 KC_HOME
#elif defined(KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11)
#    define FN1 1
#    define FN2 3
#    define KC_STEP_1 KC_ESC
#    define KC_STEP_2 KC_6
#    define KC_STEP_3 KC_LSFT
#    define KC_STEP_4 KC_5
#    define KC_STEP_5 KC_EQL
#    define KC_STEP_6 KC_M
#    define KC_STEP_7 KC_RIGHT
#    define KC_STEP_8 KC_HOME
#elif defined(KEYBOARD_keychron_q60_q60_ansi_stm32l432)
#    define FN1 1
#    define FN2 2
#    define KC_STEP_1 KC_J
#    define KC_STEP_2 KC_Z
#    define KC_STEP_3 KC_B
#    define KC_STEP_4 KC_L
#else
#    define FN1 1
#    define FN2 3
#    define KC_STEP_1 KC_J
#    define KC_STEP_2 KC_Z
#    define KC_STEP_3 KC_RIGHT
#    define KC_STEP_4 KC_HOME
#endif

// clang-format on

enum {
    LED_TEST_MODE_OFF,
    LED_TEST_MODE_WHITE,
    LED_TEST_MODE_RED,
    LED_TEST_MODE_GREEN,
    LED_TEST_MODE_BLUE,
    LED_TEST_MODE_MAX
}led_test_mode;

enum {
    FACTORY_TEST_CMD_BACKLIGHT = 0x01,
    FACTORY_TEST_CMD_OS_SWITCH,
    FACTORY_TEST_CMD_JUMP_TO_BL,
    FACTORY_TEST_CMD_EEPROM_CLEAR
};

enum {
    OS_SWITCH = 0x01,
};

uint16_t key_press_status = 0;
uint32_t timer_3s_buffer = 0;
uint32_t timer_300ms_buffer = 0;
uint8_t factory_reset_count = 0;
bool report_os_sw_state = false;
extern matrix_row_t matrix[MATRIX_ROWS];

#if defined(KEYBOARD_keychron_q0_q0_stm32l432)
bool process_record_ft(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MO(FN1):
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_FN;
            } else {
                key_press_status &= ~KEY_PRESS_FN;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_1:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_1;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = sync_timer_read32();
                } else {
                }
            } else {
                rgb_matrix_decrease_sat_noeeprom();
                key_press_status &= ~KEY_PRESS_STEP_1;
                timer_3s_buffer = 0;
            }
            return false; // Skip all further processing of this key
        case KC_STEP_2:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_2;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = sync_timer_read32();
                } else {
                }
            } else {
                rgb_matrix_decrease_hue_noeeprom();
                key_press_status &= ~KEY_PRESS_STEP_2;
                timer_3s_buffer = 0;
            }
            return false; // Skip all further processing of this key
        case KC_STEP_3:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_3;
                if (led_test_mode) {
                    if (++led_test_mode >= LED_TEST_MODE_MAX) {
                        led_test_mode = LED_TEST_MODE_WHITE;
                    }
                } else if (key_press_status == KEY_PRESS_LED_TEST) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_3;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_4:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_4;
                if (led_test_mode) {
                    led_test_mode = LED_TEST_MODE_OFF;
                } else if (key_press_status == KEY_PRESS_LED_TEST) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_4;
                timer_3s_buffer = 0;
            }
            return true;
        default:
            return true;
    }
}
#elif (defined(KEYBOARD_keychron_q9_q9_ansi_stm32l432)       || \
    defined(KEYBOARD_keychron_q9_q9_ansi_stm32l432_ec11)     || \
    defined(KEYBOARD_keychron_q9_q9_iso_stm32l432)           || \
    defined(KEYBOARD_keychron_q9_q9_iso_stm32l432_ec11))
bool process_record_ft(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MO(FN1):
        case MO(FN2):
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_FN;
            } else {
                key_press_status &= ~KEY_PRESS_FN;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_1:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_1;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_1;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_2:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_2;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = sync_timer_read32();
                } else {
                    rgb_matrix_step_reverse();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_2;
                timer_3s_buffer = 0;
            }
            return false; // Skip all further processing of this key
        case KC_STEP_3:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_3;
                if (led_test_mode) {
                    if (++led_test_mode >= LED_TEST_MODE_MAX) {
                        led_test_mode = LED_TEST_MODE_WHITE;
                    }
                } else if (key_press_status == KEY_PRESS_LED_TEST) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_3;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_4:
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_4;
                if (led_test_mode) {
                    led_test_mode = LED_TEST_MODE_OFF;
                } else if (key_press_status == KEY_PRESS_LED_TEST) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_4;
                timer_3s_buffer = 0;
            }
            return true;
        default:
            return true;
    }
}
#else
bool process_record_ft(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MO(FN1):
        case MO(FN2):
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_FN;
            } else {
                key_press_status &= ~KEY_PRESS_FN;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_1:
#if defined(KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11)
        case KC_STEP_5:
#endif // KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_1;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_1;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_2:
#if defined(KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11)
        case KC_STEP_6:
#endif // KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_2;
                if (key_press_status == KEY_PRESS_FACTORY_RESET) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_2;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_3:
#if defined(KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11)
        case KC_STEP_7:
#endif // KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_3;
                if (led_test_mode) {
                    if (++led_test_mode >= LED_TEST_MODE_MAX) {
                        led_test_mode = LED_TEST_MODE_WHITE;
                    }
                } else if (key_press_status == KEY_PRESS_LED_TEST) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_3;
                timer_3s_buffer = 0;
            }
            return true;
        case KC_STEP_4:
#if defined(KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11)
        case KC_STEP_8:
#endif // KEYBOARD_keychron_q11_q11_ansi_stm32l432_ec11
            if (record->event.pressed) {
                key_press_status |= KEY_PRESS_STEP_4;
                if (led_test_mode) {
                    led_test_mode = LED_TEST_MODE_OFF;
                } else if (key_press_status == KEY_PRESS_LED_TEST) {
                    timer_3s_buffer = sync_timer_read32();
                }
            } else {
                key_press_status &= ~KEY_PRESS_STEP_4;
                timer_3s_buffer = 0;
            }
            return true;
        default:
            return true;
    }
}
#endif

static void factory_reset(void) {
    timer_300ms_buffer = sync_timer_read32();
    factory_reset_count++;
    layer_state_t default_layer_tmp = default_layer_state;
    eeconfig_init();
    default_layer_set(default_layer_tmp);
    led_test_mode = LED_TEST_MODE_OFF;
#ifdef LED_MATRIX_ENABLE
    if (!led_matrix_is_enabled()) {
        led_matrix_enable();
    }
    led_matrix_init();
#endif
#ifdef RGB_MATRIX_ENABLE
    if (!rgb_matrix_is_enabled()) {
        rgb_matrix_enable();
    }
    rgb_matrix_init();
#endif
}

static void timer_3s_task(void) {
    if (sync_timer_elapsed32(timer_3s_buffer) > 3000) {
        timer_3s_buffer = 0;
        if (key_press_status == KEY_PRESS_FACTORY_RESET) {
            factory_reset();
        } else if (key_press_status == KEY_PRESS_LED_TEST) {
            led_test_mode = LED_TEST_MODE_WHITE;
#ifdef LED_MATRIX_ENABLE
            if (!led_matrix_is_enabled()) {
                led_matrix_enable();
            }
#endif // LED_MATRIX_ENABLE
#ifdef RGB_MATRIX_ENABLE
            if (!rgb_matrix_is_enabled()) {
                rgb_matrix_enable();
            }
#endif // RGB_MATRIX_ENABLE
        }
        key_press_status = 0;
    }
}

static void timer_300ms_task(void) {
    if (timer_300ms_buffer && sync_timer_elapsed32(timer_300ms_buffer) > 300) {
        if (factory_reset_count++ > 6) {
            timer_300ms_buffer = 0;
            factory_reset_count = 0;
        } else {
            timer_300ms_buffer = sync_timer_read32();
        }
    }
}

#ifdef LED_MATRIX_ENABLE
bool led_matrix_indicators_advanced_ft(uint8_t led_min, uint8_t led_max) {
    if (factory_reset_count) {
        for (uint8_t i = led_min; i <= led_max; i++) {
            led_matrix_set_value(i, factory_reset_count % 2 ? 0 : UINT8_MAX);
        }
    }
    return true;
}
#endif // LED_MATRIX_ENABLE

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_ft(uint8_t led_min, uint8_t led_max) {
    if (factory_reset_count) {
        for (uint8_t i = led_min; i <= led_max; i++) {
            rgb_matrix_set_color(i, factory_reset_count % 2 ? 0 : RGB_RED);
        }
    } else if (led_test_mode) {
        switch (led_test_mode) {
            case LED_TEST_MODE_WHITE:
                for (uint8_t i = led_min; i <= led_max; i++) {
                    rgb_matrix_set_color(i, RGB_WHITE);
                }
                break;
            case LED_TEST_MODE_RED:
                for (uint8_t i = led_min; i <= led_max; i++) {
                    rgb_matrix_set_color(i, RGB_RED);
                }
                break;
            case LED_TEST_MODE_GREEN:
                for (uint8_t i = led_min; i <= led_max; i++) {
                    rgb_matrix_set_color(i, RGB_GREEN);
                }
                break;
            case LED_TEST_MODE_BLUE:
                for (uint8_t i = led_min; i <= led_max; i++) {
                    rgb_matrix_set_color(i, RGB_BLUE);
                }
                break;
            default:
                break;
        }
    }
    return true;
}
#endif // RGB_MATRIX_ENABLE

void housekeeping_task_ft(void) {
    if (timer_3s_buffer) {
        timer_3s_task();
    }
    if (timer_300ms_buffer) {
        timer_300ms_task();
    }
}

static void system_switch_state_report(uint8_t index, bool active) {
    uint16_t checksum = 0;
    uint8_t data[RAW_EPSIZE] = {0};
    uint8_t payload[3] = { 0 };

    if (report_os_sw_state) {
        payload[0] = FACTORY_TEST_CMD_OS_SWITCH;
        payload[1] = OS_SWITCH;
#if defined(OS_SWITCH_REVERSE)
        payload[2] = !active;
#else
        payload[2] = active;
#endif
        data[0] = 0xAB;
        memcpy(&data[1], payload, 3);
        for (uint8_t i=1; i<RAW_EPSIZE-3; i++ ) {
            checksum += data[i];
        }
        data[RAW_EPSIZE-2] = checksum & 0xFF;
        data[RAW_EPSIZE-1] = (checksum >> 8) & 0xFF;
        raw_hid_send(data, RAW_EPSIZE);
    }
}

#ifdef RAW_ENABLE
bool dip_switch_update_ft(uint8_t index, bool active) {
    /* Send default layer state to host */
    system_switch_state_report(index, active);
    return true;
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (data[0] == 0xAB) {
        uint16_t checksum = 0;
        for (uint8_t i = 1; i < RAW_EPSIZE - 3; i++) {
            checksum += data[i];
        }
        /* Verify checksum */
        if ((checksum & 0xFF) != data[RAW_EPSIZE - 2] || checksum >> 8 != data[RAW_EPSIZE - 1]) {
            return;
        }
        switch (data[1]) {
            case FACTORY_TEST_CMD_BACKLIGHT:
                led_test_mode   = data[2];
                timer_3s_buffer = 0;
                break;
            case FACTORY_TEST_CMD_OS_SWITCH:
                report_os_sw_state = data[2];
                // if (report_os_sw_state) {
                //     dip_switch_read(true);
                // }
                break;
            case FACTORY_TEST_CMD_JUMP_TO_BL:
                if (matrix[0] & 0x1 && matrix[MATRIX_ROWS - 1] & (0x1 << (MATRIX_COLS - 1))) {
                    if (memcmp(&data[2], "JumpToBootloader", strlen("JumpToBootloader")) == 0) bootloader_jump();
                }
                break;
            case FACTORY_TEST_CMD_EEPROM_CLEAR:
                if (matrix[0] & 0x1 && matrix[MATRIX_ROWS - 1] & (0x1 << (MATRIX_COLS - 1))) {
                    if (data[2]) {
                        factory_reset();
                    }
                }
                break;
        }
    }
}
#endif // RAW_ENABLE
