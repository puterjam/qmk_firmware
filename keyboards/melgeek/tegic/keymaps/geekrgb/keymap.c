/* Copyright 2020 MelGeek <melgeek001365@gmail.com>
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

enum custom_keycodes {
#ifdef VIA_ENABLE
    KC_GEEKRGB_MODE = USER00,
#else
    KC_GEEKRGB_MODE = SAFE_RANGE,
#endif
    KC_MISSION_CONTROL,
    KC_LAUNCHPAD
};

#define KC_GR_MO KC_GEEKRGB_MODE
#define KC_MCTL KC_MISSION_CONTROL
#define KC_LPAD KC_LAUNCHPAD

#define WIN     DF(WIN_BASE)
#define MACOS   DF(MAC_BASE)

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

enum layers{
    WIN_BASE,
    WIN_FN,
    MAC_BASE,
    MAC_FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[WIN_BASE] = LAYOUT_tegic_ansi( /* Base */
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,   KC_F7,    KC_F8,    KC_F9,    KC_F10,    KC_F11,   KC_F12,   KC_PSCR,  KC_SLCK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,    KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,    KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,  KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,    KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,   KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                      KC_SPC,								 KC_RALT,  MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
	),
	[WIN_FN] = LAYOUT_tegic_ansi( /* FN */
	    MACOS  ,  KC_BRID,  KC_BRIU,   KC_TASK,  KC_FLXP, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,   _______,  RGB_TOG,
	    _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______,  _______,   KC_DEL,   _______,  KC_END,   _______,
	    KC_GR_MO,  _______,  _______,  RGB_HUI,  RGB_HUD,  RGB_SAI,  RGB_SAD, RGB_VAI,  RGB_VAD,  RGB_MOD,  _______,   _______,  _______,  KC_INS,   _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______,  _______,   _______,  _______,
	    _______,  _______,  _______,  _______,  _______,  _______,  NK_TOGG, _______,  _______,  _______,  _______,	  _______,			  KC_VOLU,
	    _______,  _______,  _______,                      _______,                               _______,  _______,   _______,  KC_MPRV,  KC_VOLD,  KC_MNXT
	),
	[MAC_BASE] = LAYOUT_tegic_ansi(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI, KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,   KC_VOLD,  KC_VOLU,  KC_PSCR,  KC_SLCK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,    KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,    KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,    KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,  KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,    KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,   KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LALT,  KC_LGUI,                      KC_SPC,								 KC_RALT,  MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
	),
	[MAC_FN] = LAYOUT_tegic_ansi(
	    WIN    , KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,  _______,  RGB_TOG,
	    _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______,  _______,   KC_DEL,   _______,  KC_END,   _______,
	    KC_GR_MO,  _______,  _______,  RGB_HUI,  RGB_HUD,  RGB_SAI,  RGB_SAD, RGB_VAI,  RGB_VAD,  RGB_MOD,  _______,   _______,  _______,  KC_INS,   _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______,  _______,   _______,  _______,
	    _______,  _______,  _______,  _______,  _______,  _______,  NK_TOGG, _______,  _______,  _______,  _______,	  _______,			  KC_VOLU,
	    _______,  _______,  _______,                      _______,                               _______,  _______,   _______,  KC_MPRV,  KC_VOLD,  KC_MNXT
	)
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 switch (keycode) {
        case KC_GEEKRGB_MODE:
            if (record->event.pressed) {
                rgb_matrix_mode(RGB_MATRIX_GEEKRGB);
                geekrgb_reload_openrgb_anim();
            }
            return false;

        case KC_MISSION_CONTROL:
            if (record->event.pressed) {
                host_consumer_send(0x29F);
            } else {
                host_consumer_send(0);
            }
            return false;
        case KC_LAUNCHPAD:
            if (record->event.pressed) {
                host_consumer_send(0x2A0);
            } else {
                host_consumer_send(0);
            }
            return false;

        default:
            return true;
    }
}

// void keyboard_post_init_user(void) {
//   // Customise these values to desired behaviour
//   debug_enable=true;
//   debug_matrix=false;
//   //debug_keyboard=true;
//   //debug_mouse=true;
// }
