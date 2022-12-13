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
#include "raw_hid.h"

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
