
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
    LAYER_END,
    LAYER_5,
    LAYER_6,
    LAYER_7,
    LAYER_8,
    LAYER_MOUSE,
};

/**
在这里定义的层，切到该层这个层就变为自动阻击层
*/
//#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE


typedef union {
    uint32_t raw1;
    struct {
        uint16_t auto_time: 13;
        uint8_t threshold_value : 3; //
        bool    is_auto_enabled : 1;  //新加的
    };
} auto_config_t;
//2的数字范围是从0到3， 4是16位，5是0-31位，6是即0到63。
static auto_config_t user_config;

// OLED 页面枚举
typedef enum {
    OLED_DEFAULT,   // 默认页（bingocat）
    OLED_TRACKBALL  // 轨迹球参数页
} oled_page_t;

static oled_page_t oled_current_page = OLED_DEFAULT;

void keyboard_post_init_user(void){
    user_config.raw1 = eeconfig_read_user();
}


void eeconfig_init_user(void){
    user_config.raw1 = 0;
    user_config.auto_time = 0;
    user_config.is_auto_enabled = 0;
    user_config.threshold_value = 1;
    eeconfig_update_user(user_config.raw1);
}

#define CHARYBDIS_AUTO_MAX_TIMEOUT_MS 3000
#define CHARYBDIS_AUTO_MIN_TIMEOUT_MS 0

//Charybdis轨迹球滚动停止后多长时间没操作返回原来层，默认1秒
#ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS user_config.auto_time
#endif


//Charybdis自动指针层触发阈值，值越低越灵敏 user_config.threshold_value 0-7
#ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD user_config.threshold_value
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [LAYER_BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_LOWER] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_RAISE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_POINTER] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_END] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_5] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_6] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_7] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_8] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC
    ),
    [LAYER_MOUSE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_MINS,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                          KC_LALT, XXXXXXX, KC_SPC,           KC_ENT,  XXXXXXX, KC_BSPC  )
};


// clang-format on
//用来检测自动切换鼠标层的地方，如果鼠标的水平位移x或垂直位移y的绝对值大于设定的阈值
//自动切换到LAYER_POINTER鼠标层，并将rgb改为绿色
//如果没有开启这个不起作用
#ifdef POINTING_DEVICE_ENABLE

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
//发送传感器数据的回调，以便用户代码可以拦截和修改数据。返回鼠标报告。
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if(user_config.is_auto_enabled){
//            如果鼠标的水平或垂直移动距离超过了阈值 CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
            if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
                //如果自动切换鼠标层的定时器为0
                if (auto_pointer_layer_timer == 0) {
//                    layer_on(LAYER_POINTER);
                    //打开 LAYER_MOUSE 鼠标层。
                    layer_on(LAYER_MOUSE);
                }
                //更新自动切换鼠标层的定时器
                auto_pointer_layer_timer = timer_read();
            }
    }

    return mouse_report;
}

void matrix_scan_user(void) {
    if(user_config.is_auto_enabled){
         // 如果自动切换鼠标层的定时器不为0，并且当前时间与定时器启动时间的差值大于等于 CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MSn
        if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
            // 将自动切换鼠标层的定时器重置为0
            auto_pointer_layer_timer = 0;
    //        layer_off(LAYER_POINTER);
    //关闭 LAYER_MOUSE 鼠标层
            layer_off(LAYER_MOUSE);
        };
    }

}
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE


//在图层变化时自动启用狙击模式，切换到LAYER_POINTER指针层启用阻击模式
//和自动启用阻击模式相关
#ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

//和rgb有关可以不用管
#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif


//自定义键值
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        // toggle auto mouse enable key
        case AUTO_MODE_TOGGLE:
            if(record->event.pressed) { // key down
                user_config.is_auto_enabled ^=  1;
            } // 是否开启自动切层
            return false; // prevent further processing of keycode
        case AUTO_TIME_50:
            if(record->event.pressed) { // key down
            user_config.auto_time += 50;
            if(user_config.auto_time > CHARYBDIS_AUTO_MAX_TIMEOUT_MS){
                user_config.auto_time = CHARYBDIS_AUTO_MIN_TIMEOUT_MS;
            }
            } // 增加轨迹球超时时间
            return false; // prevent further processing of keycode
        case AUTO_TIME_50R:
            if(record->event.pressed) { // key down
            user_config.auto_time -= 50;
            if(user_config.auto_time > CHARYBDIS_AUTO_MAX_TIMEOUT_MS){
                user_config.auto_time = CHARYBDIS_AUTO_MAX_TIMEOUT_MS;
            }
            } // 增加轨迹球超
            return false; // prevent further processing of keycode
        case AUTO_TIME_100:
            if(record->event.pressed) { // key down
            user_config.auto_time += 100;
            if(user_config.auto_time > CHARYBDIS_AUTO_MAX_TIMEOUT_MS){
                user_config.auto_time = 0;
            }
            } // 增加轨迹球超时时间
            return false; // prevent further processing of keycode
        case AUTO_THRESHOLD:
            if(record->event.pressed) { // key down
                user_config.threshold_value +=  1;
//                eeconfig_update_user(user_config.raw1);
            } // 增加轨迹球触发阈值
            return false; // prevent further processing of keycode
        case TInfo:
            if(record->event.pressed) { // key down
                oled_current_page = (oled_current_page == OLED_DEFAULT) ?
                                    OLED_TRACKBALL : OLED_DEFAULT;
            } // 显示当前轨迹球的设置
            return false; // prevent further processing of keycode
        case T_SAVE:
            if(record->event.pressed) { // key down
                eeconfig_update_user(user_config.raw1);
            } // 保存自动切层的设置
            return false; // prevent further processing of keycode
        }
    return true;
}

/* oled stuff :) */
#ifdef OLED_ENABLE

//270 默认
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
        return OLED_ROTATION_180;
//        return OLED_ROTATION_270;
}

static void tv_ms(void) {
//    oled_clear();
    //   鼠标模式OLED
    oled_write_P(PSTR("ATG:"), false);
    char auto_m[2];
    snprintf(auto_m, sizeof(auto_m), "%d", user_config.is_auto_enabled);
    oled_write(auto_m, false);
    //    自动切层模式
    oled_write_P(PSTR(" ATV:"), false);
    char count_atv_str[2];
    snprintf(count_atv_str, sizeof(count_atv_str), "%d", CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD);
    oled_write(count_atv_str, false);

    oled_write_P(PSTR(" AMS:"), false);
    char count_ams_str[6];
    snprintf(count_ams_str, sizeof(count_ams_str), "%d", CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS);
    oled_write_ln(count_ams_str, false);

    trackball_oled_default();

    //   滚动/阻击模式OLED
    trackball_oled_info();
    }


void render_mod_status_gui_alt1(uint8_t modifiers) {
static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }
}

void render_mod_status_gui_alt2(uint8_t modifiers) {
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};


    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};


    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}
void render_mod_status_ctrl_shift1(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }
}

void render_mod_status_ctrl_shift2(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}

static void render_default(void) {


//    render_mod_status_gui_alt(get_mods());
//    render_mod_status_ctrl_shift(get_mods());

//    oled_write_ln("C35 Ball", false);
    oled_write("C35 Ball   ", false);
    render_mod_status_gui_alt1(get_mods());
    render_mod_status_ctrl_shift1(get_mods());

    oled_write("LAYER:", false);

    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE:
            oled_write("  0  ", false);
            break;
        case LAYER_LOWER:
            oled_write("  1  ", false);
            break;
        case LAYER_RAISE:
            oled_write("  2  ", false);
            break;
        case LAYER_POINTER:
            oled_write("3", false);
            break;
        case LAYER_END:
            oled_write("  4  ", false);
            break;
        case LAYER_5:
            oled_write("  5  ", false);
            break;
        case LAYER_6:
            oled_write("  6  ", false);
            break;
        case LAYER_7:
            oled_write("  7  ", false);
            break;
        case LAYER_8:
            oled_write("  8  ", false);
            break;
        case LAYER_MOUSE:
            oled_write("Mouse", false);
            break;
        default:
            oled_write("Undef", false);
    }

    render_mod_status_gui_alt2(get_mods());
    render_mod_status_ctrl_shift2(get_mods());

    uint16_t m = get_current_wpm();
    oled_write_P(PSTR("WPM:"), false);
    char wpm_str[6];
    snprintf(wpm_str, sizeof(wpm_str), "%d", m);
    oled_write_ln(wpm_str, false);
    oled_write_ln("", false);

}


// 用户自定义OLED任务处理函数
bool oled_task_user(void) {
//tv_ms();
//    oled_clear();
    switch (oled_current_page) {
        case OLED_DEFAULT:
            render_default();
            break;
        case OLED_TRACKBALL:
            tv_ms();
            break;
         }
    return false;
}


#endif



//旋钮映射需要在vial的rules.mk加入
//ENCODER_ENABLE = yes
//ENCODER_MAP_ENABLE = yes
//旋钮要(0,0 0,1)  (1,0 1,1) 这里前一个代表旋钮下标，后一个都一样，有两个旋钮，前面就是0 1
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [4] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [5] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [6] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [7] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [8] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [9] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
};
