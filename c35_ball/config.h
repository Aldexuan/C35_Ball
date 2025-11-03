// Copyright 2022 aki27 (@aki27kbd)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#define OLED_FONT_H "keyboards/c35_ball/keymaps/vial/glcdfont.c"
#define OLED_TIMEOUT 60000





/* RGB MATRIX */
#ifdef RGB_MATRIX_ENABLE
#    define RGBLED_NUM 38
#    define DRIVER_LED_TOTAL RGBLED_NUM
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 180
#    define RGB_DISABLE_WHEN_USB_SUSPENDED
#    define RGB_MATRIX_KEYPRESSES

// Startup values.
#    define RGB_MATRIX_DEFAULT_VAL 64

// Rainbow swirl as startup mode.
#    define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT

// Slow swirl at startup.
#    define RGB_MATRIX_DEFAULT_SPD 32

#    ifndef __arm__
// Disable control of RGB matrix by keycodes (must use firmware implementation
// to control the feature).
#        define RGB_MATRIX_DISABLE_KEYCODES
#    else // __arm__
// Enable all animations on ARM boards since they have plenty of memory
// available for it.
#        define ENABLE_RGB_MATRIX_ALPHAS_MODS
#        define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#        define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#        define ENABLE_RGB_MATRIX_BREATHING
#        define ENABLE_RGB_MATRIX_BAND_SAT
#        define ENABLE_RGB_MATRIX_BAND_VAL
#        define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#        define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#        define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#        define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#        define ENABLE_RGB_MATRIX_CYCLE_ALL
#        define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#        define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#        define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#        define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#        define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#        define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#        define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#        define ENABLE_RGB_MATRIX_DUAL_BEACON
#        define ENABLE_RGB_MATRIX_RAINBOW_BEACON
#        define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#        define ENABLE_RGB_MATRIX_RAINDROPS
#        define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#        define ENABLE_RGB_MATRIX_HUE_BREATHING
#        define ENABLE_RGB_MATRIX_HUE_PENDULUM
#        define ENABLE_RGB_MATRIX_HUE_WAVE
#        define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#        define ENABLE_RGB_MATRIX_DIGITAL_RAIN
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#        define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#        define ENABLE_RGB_MATRIX_SPLASH
#        define ENABLE_RGB_MATRIX_MULTISPLASH
#        define ENABLE_RGB_MATRIX_SOLID_SPLASH
#        define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#    endif // !__arm__
#endif


/* Trackball */
#    undef RP_SPI_USE_SPI0
#    define RP_SPI_USE_SPI0 TRUE
#    undef RP_SPI_USE_SPI1
#    define RP_SPI_USE_SPI1 FALSE
#    define SPI_DRIVER SPID0

#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23
#define PMW33XX_CS_PIN GP16


/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
//自动指针层，指针层可以在移动轨迹球时自动启用。
// Automatically enable the pointer layer when moving the trackball.  See also:
//触发敏感度也可以调整。值越低，触发越敏感
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
 #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
//旋转角度，改变球的角度 旋转角度仅支持 -127 到 127。
//#undef ROTATIONAL_TRANSFORM_ANGLE
//#define ROTATIONAL_TRANSFORM_ANGLE 90
#define POINTING_DEVICE_ROTATION_90
//（可选）将 X 和 Y 数据旋转 90 度。
//这是一个 预定义的常量，代表将指点设备的坐标轴 顺时针旋转 90 度。
//作用：当你把一个 Trackball 或 Trackpoint 安装时旋转了 90 度（相对于默认方向），QMK 默认的 X 和 Y 轴可能就不对了
//使用这个宏可以让 QMK 自动将 X 和 Y 坐标进行交换并调整方向，使移动方向与你的物理操作一致。
//等效变换：
//原始 X 轴 → 新的 Y 轴（方向可能反转）
//原始 Y 轴 → 新的 X 轴（方向可能反转）
//常见值：
//POINTING_DEVICE_ROTATION_0：不旋转（默认）
//POINTING_DEVICE_ROTATION_90：顺时针旋转 90°
//POINTING_DEVICE_ROTATION_180：旋转 180°
//POINTING_DEVICE_ROTATION_270：顺时针旋转 270°（或逆时针 90°）
//#define POINTING_DEVICE_ROTATION_90



//反转Y轴数据
//#define POINTING_DEVICE_INVERT_Y
//反转X轴数据
#define POINTING_DEVICE_INVERT_X
//要反转水平滚动方向，定义CHARYBDIS_DRAGSCROLL_REVERSE_X：
//#define CHARYBDIS_DRAGSCROLL_REVERSE_X
//要反转垂直滚动方向（即模仿 macOS“自然”滚动方向），请定义CHARYBDIS_DRAGSCROLL_REVERSE_Y
//#define CHARYBDIS_DRAGSCROLL_REVERSE_Y


// Enable pointer acceleration, which increases the speed by ~2x for large
// displacement, while maintaining 1x speed for slow movements.
// - `CHARYBDIS_POINTER_ACCELERATION_FACTOR`
//启用指针加速，这将使速度增加~2x
//位移，同时保持1倍的速度缓慢移动。
//#   define CHARYBDIS_POINTER_ACCELERATION_ENABLE

// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
// #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif  // POINTING_DEVICE_ENABLE

