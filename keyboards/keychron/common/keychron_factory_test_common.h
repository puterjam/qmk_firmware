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

#include "stdint.h"

bool process_record_ft(uint16_t keycode, keyrecord_t *record);
void housekeeping_task_ft(void);

#ifdef LED_MATRIX_ENABLE
bool led_matrix_indicators_advanced_ft(uint8_t led_min, uint8_t led_max);
#endif // LED_MATRIX_ENABLE

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_ft(uint8_t led_min, uint8_t led_max);
#endif // RGB_MATRIX_ENABLE

#ifdef RAW_ENABLE
bool dip_switch_update_ft(uint8_t index, bool active);
#endif // RAW_ENABLE
