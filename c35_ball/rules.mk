# MCU name
# MCU = RP2040

# Bootloader selection
# BOOTLOADER = rp2040
ALLOW_WARNINGS = yes
PICO_INTRINSICS_ENABLED = no # ATM Unsupported by ChibiOS.
# Build Options
#   change yes to no to disable
#
# ENCODER_ENABLE = yes
# BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
# MOUSEKEY_ENABLE = yes       # Mouse keys
# EXTRAKEY_ENABLE = yes       # Audio control and System control
# CONSOLE_ENABLE = no         # Console for debug
# COMMAND_ENABLE = no         # Commands for debug and configuration
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
RGB_MATRIX_SUPPORTED = yes  # RGB matrix is supported and enabled by default
RGBLIGHT_SUPPORTED = no    # RGB underglow is supported, but not enabled by default
RGB_MATRIX_ENABLE = yes     # Enable keyboard RGB matrix functionality


# CUSTOM_MATRIX = lite
# SRC += matrix.c

POINTING_DEVICE_ENABLE = yes
# POINTING_DEVICE_DRIVER = pmw3360
POINTING_DEVICE_DRIVER = pmw3389

OLED_ENABLE     = yes

#VIA_ENABLE = yes


# Disable unnecessary functions to reduce firmware size
# SPACE_CADET_ENABLE = no
# MAGIC_ENABLE = no
