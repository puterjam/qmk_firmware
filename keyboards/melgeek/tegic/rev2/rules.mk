# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes         # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no          # Mouse keys
EXTRAKEY_ENABLE = no          # Audio control and System control
CONSOLE_ENABLE = yes            # Console for debug
COMMAND_ENABLE = no            # Commands for debug and configuration
NKRO_ENABLE = no              # Enable N-Key Rollover
BACKLIGHT_ENABLE = no          # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no           # Enable keyboard RGB underglow
RGB_MATRIX_ENABLE = yes        # Use RGB matrix
RGB_MATRIX_DRIVER = IS31FL3741
JOYSTICK_ENABLE = no
MIDI_ENABLE = no

LTO_ENABLE = yes
