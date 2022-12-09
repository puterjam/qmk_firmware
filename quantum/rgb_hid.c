#include "quantum.h"
#include "raw_hid.h"

void rgb_hid_receive(uint8_t *data, uint8_t length) {
    bool send = false;

    dprintf("rgb_hid_receive\n");

    switch (*data) {
#ifdef OPENRGB_ENABLE
        case 1 ... 9: {
            send = openrgb_command_handler(data, length);
            break;
        }
#endif
#ifdef SIGNALRGB_ENABLE
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
