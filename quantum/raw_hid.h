#pragma once

void raw_hid_receive(uint8_t *data, uint8_t length);

void raw_hid_send(uint8_t *data, uint8_t length);

// HIDRGB RAW HID
void rgb_hid_send(uint8_t *data, uint8_t length);

void rgb_hid_receive(uint8_t *data, uint8_t length);
