#pragma once

enum signalrgb_commands
{
    GET_QMK_VERSION = 0x21,
    GET_PROTOCOL_VERSION,
    GET_UNIQUE_IDENTIFIER,
    STREAM_RGB_DATA,
    SET_SIGNALRGB_MODE_ENABLE,
    SET_SIGNALRGB_MODE_DISABLE,
    GET_TOTAL_LEDS,
    GET_FIRMWARE_TYPE,
};

enum signalrgb_responses //These are a bit clunky right now. Could use improvement.
{
    PROTOCOL_VERSION_BYTE_1 = 1,
    PROTOCOL_VERSION_BYTE_2 = 0,
    PROTOCOL_VERSION_BYTE_3 = 4,
    QMK_VERSION_BYTE_1 = 0,
    QMK_VERSION_BYTE_2 = 17,
    QMK_VERSION_BYTE_3 = 5,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_1 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_2 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_3 = 0,
    FIRMWARE_TYPE_BYTE = 1,
    DEVICE_ERROR_LEDS = 255, //Error code to show that there are more leds than a packet will allow.
};

// extern RGB g_signalrgb_mode_colors[DRIVER_LED_TOTAL];

bool signal_rgb_command_handler(uint8_t *data, uint8_t length);
void get_qmk_version(void);
void get_signalrgb_protocol_version(void);
void get_unique_identifier(void);
void led_streaming(uint8_t *data);
void signalrgb_mode_enable(void);
void signalrgb_mode_disable(void);
void signalrgb_total_leds(void);
void signalrgb_firmware_type(void);

//Changelogs for Firmware Versions------------------------------------
//V1.0.1 added detection for the total number of LEDs a board has. Plugins need a rewrite to make use of this change. Rewritten plugins will not function with older firmware.
//V1.0.2 added another detection byte for which fork of SignalRGB firmware device is running. This means we can keep overlap Unique Identifiers between Sonix and Mainline.
//V1.0.3 unifies the command IDs between Mainline, Sonix, and VIA. All commands have a 0x20 offset now.
//V1.0.4 improves detection handling, and has a complete rewrite of the plugins. Also merges Mainline and VIA branches. VIA Branch is deprecated.
