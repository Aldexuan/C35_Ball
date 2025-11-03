#include "quantum.h"
#include <math.h>
#include "c35_ball.h"
#include "wait.h"
#include "debug.h"
#include <stdio.h>
#include "pointing_device.h"
extern const pointing_device_driver_t pointing_device_driver;


#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif // CONSOLE_ENABLE

#ifdef POINTING_DEVICE_ENABLE
#    ifndef CHARYBDIS_MINIMUM_DEFAULT_DPI
#        define CHARYBDIS_MINIMUM_DEFAULT_DPI 400
#    endif // CHARYBDIS_MINIMUM_DEFAULT_DPI

#    ifndef CHARYBDIS_DEFAULT_DPI_CONFIG_STEP
#        define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200
#    endif // CHARYBDIS_DEFAULT_DPI_CONFIG_STEP

#    ifndef CHARYBDIS_MINIMUM_SNIPING_DPI
#        define CHARYBDIS_MINIMUM_SNIPING_DPI 200
#    endif // CHARYBDIS_MINIMUM_SNIPER_MODE_DPI

#    ifndef CHARYBDIS_SNIPING_DPI_CONFIG_STEP
#        define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 100
#    endif // CHARYBDIS_SNIPING_DPI_CONFIG_STEP


#    ifndef CHARYBDIS_DRAGSCROLL_BUFFER_SIZE
#        define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 6
#    endif // !CHARYBDIS_DRAGSCROLL_BUFFER_SIZE


#ifndef T_ROTATION_ANGLE
#define T_ROTATION_ANGLE { -75, -60, -45, -30, -15, 0, 15, 30, 45, 60, 75 }
#endif


static const int16_t angle_array[] = T_ROTATION_ANGLE;
#define ANGLE_SIZE (sizeof(angle_array) / sizeof(int16_t))



typedef union {
    uint64_t raw;
    struct {
        uint16_t pointer_dragscroll_dpi : 9; // 0-511
        uint8_t pointer_default_dpi : 4; // 16 steps available.
        uint8_t pointer_sniping_dpi : 2; // 4 steps available.
        uint8_t rotation_angle_idx;
        bool    is_dragscroll_enabled : 1;
        bool    is_sniping_enabled : 1;
    } __attribute__((packed));
} charybdis_config_t;

static charybdis_config_t g_charybdis_config = {0};
//static charybdis_config_t g_charybdis_config;


static void read_charybdis_config_from_eeprom(charybdis_config_t* config) {
    config->raw                   = eeconfig_read_kb();
    config->pointer_dragscroll_dpi = (config->pointer_dragscroll_dpi == 0) ? 100 : config->pointer_dragscroll_dpi;
    config->is_dragscroll_enabled = false;
    config->is_sniping_enabled    = false;
}


// 旋转矩阵缓存（避免每次计算 sin/cos）
static float g_cos_theta = 1.0f;
static float g_sin_theta = 0.0f;
// 预计算当前角度的 cos/sin
static void update_rotation_transform(void) {
    float rad = (float)angle_array[g_charybdis_config.rotation_angle_idx] * (M_PI / 180.0f) * -1;
    g_cos_theta = cosf(rad);
    g_sin_theta = sinf(rad);
}



static void write_charybdis_config_to_eeprom(charybdis_config_t* config) {
    eeconfig_update_kb(config->raw);
}

/** \brief Return the current value of the pointer's default DPI. */
static uint16_t get_pointer_default_dpi(charybdis_config_t* config) {
    return (uint16_t)config->pointer_default_dpi * CHARYBDIS_DEFAULT_DPI_CONFIG_STEP + CHARYBDIS_MINIMUM_DEFAULT_DPI;
}

/** \brief Return the current value of the pointer's sniper-mode DPI. */
static uint16_t get_pointer_sniping_dpi(charybdis_config_t* config) {
    return (uint16_t)config->pointer_sniping_dpi * CHARYBDIS_SNIPING_DPI_CONFIG_STEP + CHARYBDIS_MINIMUM_SNIPING_DPI;
}


/** \brief Set the appropriate DPI for the input config. */
static void maybe_update_pointing_device_cpi(charybdis_config_t* config) {
    if (config->is_dragscroll_enabled) {
        pointing_device_set_cpi(g_charybdis_config.pointer_dragscroll_dpi);
    } else if (config->is_sniping_enabled) {
        pointing_device_set_cpi(get_pointer_sniping_dpi(config));
    } else {
        pointing_device_set_cpi(get_pointer_default_dpi(config));
    }
}

/**
 * The increment/decrement steps are equal to CHARYBDIS_DEFAULT_DPI_CONFIG_STEP.
 */
static void step_pointer_default_dpi(charybdis_config_t* config, bool forward) {
    config->pointer_default_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}
/**
 * The increment/decrement steps are equal to CHARYBDIS_SNIPING_DPI_CONFIG_STEP.
 */
static void step_pointer_sniping_dpi(charybdis_config_t* config, bool forward) {
    config->pointer_sniping_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}



uint16_t charybdis_get_pointer_default_dpi(void) {
    return get_pointer_default_dpi(&g_charybdis_config);
}

uint16_t charybdis_get_pointer_sniping_dpi(void) {
    return get_pointer_sniping_dpi(&g_charybdis_config);
}

void charybdis_cycle_pointer_default_dpi_noeeprom(bool forward) {
    step_pointer_default_dpi(&g_charybdis_config, forward);
}


void charybdis_cycle_pointer_default_dpi(bool forward) {
    step_pointer_default_dpi(&g_charybdis_config, forward);
    write_charybdis_config_to_eeprom(&g_charybdis_config);
}

void charybdis_cycle_pointer_sniping_dpi(bool forward) {
    step_pointer_sniping_dpi(&g_charybdis_config, forward);
    write_charybdis_config_to_eeprom(&g_charybdis_config);
}

bool charybdis_get_pointer_sniping_enabled(void) {
    return g_charybdis_config.is_sniping_enabled;
}


void charybdis_set_pointer_sniping_enabled(bool enable) {
    g_charybdis_config.is_sniping_enabled = enable;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
}

bool charybdis_get_pointer_dragscroll_enabled(void) {
    return g_charybdis_config.is_dragscroll_enabled;
}

void charybdis_set_pointer_dragscroll_enabled(bool enable) {
    g_charybdis_config.is_dragscroll_enabled = enable;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
}

void charybdis_cycle_pointer_sniping_dpi_noeeprom(bool forward) {
    step_pointer_sniping_dpi(&g_charybdis_config, forward);
}
/**
 * \brief Augment the pointing device behavior.
 *
 * Implement drag-scroll.
 */
static void pointing_device_task_charybdis(report_mouse_t* mouse_report) {
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    if (g_charybdis_config.is_dragscroll_enabled) {
    #ifdef CHARYBDIS_DRAGSCROLL_REVERSE_X
        scroll_buffer_x -= mouse_report->x;
    #else
        scroll_buffer_x += mouse_report->x;
    #endif // CHARYBDIS_DRAGSCROLL_REVERSE_X

    #ifdef CHARYBDIS_DRAGSCROLL_REVERSE_Y
        scroll_buffer_y -= mouse_report->y;
    #else
        scroll_buffer_y += mouse_report->y;
    #endif // CHARYBDIS_DRAGSCROLL_REVERSE_Y
        mouse_report->x = 0;
        mouse_report->y = 0;
        if (abs(scroll_buffer_x) > CHARYBDIS_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
            scroll_buffer_x = 0;
        }
        if (abs(scroll_buffer_y) > CHARYBDIS_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
            scroll_buffer_y = 0;
        }
    }
    // 当未启用拖拽滚动，且旋转角度索引不为0（表示需要旋转）时，进行坐标变换
    else if (g_charybdis_config.rotation_angle_idx != 0) {
        float x = (float)mouse_report->x;
        float y = (float)mouse_report->y;
        float rx = x * g_cos_theta - y * g_sin_theta;
        float ry = x * g_sin_theta + y * g_cos_theta;

        // 限制在 int8_t 范围内 [-127, 127]
        mouse_report->x = (int8_t)fminf(fmaxf(roundf(rx), -127.0f), 127.0f);
        mouse_report->y = (int8_t)fminf(fmaxf(roundf(ry), -127.0f), 127.0f);
    }
    // else: 如果 is_dragscroll_enabled 为 false 且 rotation_angle_idx == 0，则不做任何处理（保持 mouse_report->x/y 不变）

}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    if (is_keyboard_master()) {
        pointing_device_task_charybdis(&mouse_report);
        mouse_report = pointing_device_task_user(mouse_report);
    }
    return mouse_report;
}

#    if defined(POINTING_DEVICE_ENABLE) && !defined(NO_CHARYBDIS_KEYCODES)
/** \brief Whether SHIFT mod is enabled. */
static bool has_shift_mod(void) {
#        ifdef NO_ACTION_ONESHOT
    return mod_config(get_mods()) & MOD_MASK_SHIFT;
#        else
    return mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
#        endif // NO_ACTION_ONESHOT
}
#    endif // POINTING_DEVICE_ENABLE && !NO_CHARYBDIS_KEYCODES

/**
 * \brief Outputs the Charybdis configuration to console.
 *
 * Prints the in-memory configuration structure to console, for debugging.
 * Includes:
 *   - raw value
 *   - drag-scroll: on/off
 *   - sniping: on/off
 *   - default DPI: internal table index/actual DPI
 *   - sniping DPI: internal table index/actual DPI
 */
static void debug_charybdis_config_to_console(charybdis_config_t* config) {
#    ifdef CONSOLE_ENABLE
    dprintf("(charybdis) process_record_kb: config = {\n"
            "\traw = 0x%X,\n"
            "\t{\n"
            "\t\tis_dragscroll_enabled=%u\n"
            "\t\tis_sniping_enabled=%u\n"
            "\t\tdefault_dpi=0x%X (%u)\n"
            "\t\tsniping_dpi=0x%X (%u)\n"
            "\t}\n"
            "}\n",
            config->raw, config->is_dragscroll_enabled, config->is_sniping_enabled, config->pointer_default_dpi, get_pointer_default_dpi(config), config->pointer_sniping_dpi, get_pointer_sniping_dpi(config));
#    endif // CONSOLE_ENABLE
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (!process_record_user(keycode, record)) {
        debug_charybdis_config_to_console(&g_charybdis_config);
        return false;
    }
#    ifdef POINTING_DEVICE_ENABLE
#ifndef NO_CHARYBDIS_KEYCODES
    switch (keycode) {
        case POINTER_DEFAULT_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                charybdis_cycle_pointer_default_dpi(/* forward= */ !has_shift_mod());
            }
            break;
        case POINTER_DEFAULT_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                charybdis_cycle_pointer_default_dpi(/* forward= */ has_shift_mod());
            }
            break;
        case POINTER_SNIPING_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                charybdis_cycle_pointer_sniping_dpi(/* forward= */ !has_shift_mod());
            }
            break;
        case POINTER_SNIPING_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                charybdis_cycle_pointer_sniping_dpi(/* forward= */ has_shift_mod());
            }
            break;
        case SNIPING_MODE:
            charybdis_set_pointer_sniping_enabled(record->event.pressed);
            break;
        case SNIPING_MODE_TOGGLE:
            if (record->event.pressed) {
                charybdis_set_pointer_sniping_enabled(!charybdis_get_pointer_sniping_enabled());
            }
            break;


        case POINTER_DRAGSCROLL_DPI_FORWARD:
            if (record->event.pressed) {
                uint16_t new_value = g_charybdis_config.pointer_dragscroll_dpi + 100;
                if (new_value > 511) { // 确保不超过位域的最大值511
                    g_charybdis_config.pointer_dragscroll_dpi = 100; // 超过511时重置为100
                } else if (new_value > 500) {
                    g_charybdis_config.pointer_dragscroll_dpi = 100; // 达到或超过500时也重置为100
                } else {
                    g_charybdis_config.pointer_dragscroll_dpi = new_value; // 正常增加
                }
                maybe_update_pointing_device_cpi(&g_charybdis_config);
                write_charybdis_config_to_eeprom(&g_charybdis_config);
            }
            break;
        case POINTER_DRAGSCROLL_DPI_REVERSE:
            if (record->event.pressed) {
               uint16_t new_value = g_charybdis_config.pointer_dragscroll_dpi - 100;
                if (new_value > 511) { // 确保不超过位域的最大值511
                    g_charybdis_config.pointer_dragscroll_dpi = 500; // 超过511时重置为100
                } else if (new_value > 500) {
                    g_charybdis_config.pointer_dragscroll_dpi = 500; // 达到或超过500时也重置为100
                } else {
                    g_charybdis_config.pointer_dragscroll_dpi = new_value; // 正常增加
                }
                maybe_update_pointing_device_cpi(&g_charybdis_config);
                write_charybdis_config_to_eeprom(&g_charybdis_config);
            }
            break;
        case DRAGSCROLL_MODE:
            charybdis_set_pointer_dragscroll_enabled(record->event.pressed);
            break;
        case DRAGSCROLL_MODE_TOGGLE:
            if (record->event.pressed) {
                charybdis_set_pointer_dragscroll_enabled(!charybdis_get_pointer_dragscroll_enabled());
            }
            break;
        case T_R_R:
            if (record->event.pressed) {
               g_charybdis_config.rotation_angle_idx = (g_charybdis_config.rotation_angle_idx + 1) % ANGLE_SIZE;
               update_rotation_transform();
            }
            break;
        case T_R_L:
            if (record->event.pressed) {
               g_charybdis_config.rotation_angle_idx = (ANGLE_SIZE + g_charybdis_config.rotation_angle_idx - 1) % ANGLE_SIZE;
               update_rotation_transform();
            }
            break;

    }
#        endif // !NO_CHARYBDIS_KEYCODES
#    endif     // POINTING_DEVICE_ENABLE
    if (IS_QK_KB(keycode) || IS_MOUSEKEY(keycode)) {
        debug_charybdis_config_to_console(&g_charybdis_config);
    }
    return true;
}

void eeconfig_init_kb(void) {
    g_charybdis_config.raw = 0;
    write_charybdis_config_to_eeprom(&g_charybdis_config);
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    update_rotation_transform();
    eeconfig_init_user();
}

void matrix_init_kb(void) {
    read_charybdis_config_from_eeprom(&g_charybdis_config);
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    matrix_init_user();
}

#ifdef CHARYBDIS_CONFIG_SYNC
void charybdis_config_sync_handler(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(g_charybdis_config)) {
        memcpy(&g_charybdis_config, initiator2target_buffer, sizeof(g_charybdis_config));
    }
}
#    endif

void keyboard_post_init_kb(void) {
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    update_rotation_transform();
    #ifdef CHARYBDIS_CONFIG_SYNC
        transaction_register_rpc(RPC_ID_KB_CONFIG_SYNC, charybdis_config_sync_handler);
    #endif
    keyboard_post_init_user();
}
#endif     // POINTING_DEVICE_ENABLE
bool shutdown_kb(bool jump_to_bootloader) {
    if (!shutdown_user(jump_to_bootloader)) {
        return false;
    }

#ifdef RGB_MATRIX_ENABLE
    void rgb_matrix_update_pwm_buffers(void);
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE
    return true;
}

//oled
void trackball_oled_default(void) {
    oled_write_P(PSTR("ANG:"), false);
    char angle_str[6];
    snprintf(angle_str, sizeof(angle_str), "%3d", angle_array[g_charybdis_config.rotation_angle_idx]);
    oled_write(angle_str, false); // 显示如 "  0" 或 "-45"

    char count_default_str[6];
    snprintf(count_default_str, sizeof(count_default_str), "%d", charybdis_get_pointer_default_dpi());
    oled_write_P(PSTR(" MD:"), false);
    oled_write_ln(count_default_str, false);


}
void trackball_oled_info(void) {
    char is_sniping_str[2];
    snprintf(is_sniping_str, sizeof(is_sniping_str), "%d", charybdis_get_pointer_sniping_enabled());
    oled_write_P(PSTR("SNP-T:"), false);
    oled_write(is_sniping_str, false);

    oled_write_P(PSTR(" SNP-DPI:"), false);
    char count_sniping_str[6];
    snprintf(count_sniping_str, sizeof(count_sniping_str), "%d", charybdis_get_pointer_sniping_dpi());
    oled_write_ln(count_sniping_str, false);

    char is_dragscroll_str[2];
    snprintf(is_dragscroll_str, sizeof(is_dragscroll_str), "%d", charybdis_get_pointer_dragscroll_enabled());
    oled_write_P(PSTR("DRG-T:"), false);
    oled_write(is_dragscroll_str, false);

    oled_write_P(PSTR(" DRG-DPI:"), false);
    char count_dragscroll_str[6];
    snprintf(count_dragscroll_str, sizeof(count_dragscroll_str), "%d", g_charybdis_config.pointer_dragscroll_dpi);
    oled_write_ln(count_dragscroll_str, false);

//    trackball_oled_default();
    }
