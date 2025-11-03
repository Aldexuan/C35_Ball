#include "v2.h"


// clang-format off
#ifdef RGB_MATRIX_ENABLE
/**
 * \brief LEDs index.
     0   1   2   3   4             5    6    7    8   9
 *   0   24  48  72  96            120  144  168  192 216
 ╭──────────────────────          ───────────────────
0    0   1   2   3   4              5    6    7    8   9
 ├──────────────────────          ─────────────── ───
13   19  18  17  16  15            14   13   12   11  10
 ├──────────────────────          ─────────────── ───
26   20  21   22  23  24           25   26   27   28  29
 ├──────────────────────          ──────────────────
39           35  34  33            32   31   30
 ╰──────────────────────          ───────────────────

 */
led_config_t g_led_config = {
{
    /* Key Matrix to LED index. */
    // Left split.
    {      0,       1,      2,   3,   4,             5,    6,    7,    8,       9 }, // Num row
    {      19,      18,     17,  16,  15,            14,   13,   12,   11,      10 }, // Top row
    {      20,      21,     22,  23,  24,            25,   26,   27,   28,      29 }, // Middle row
    {      NO_LED,  NO_LED, 35,  34,  33,            32,   31,   30,   NO_LED,  NO_LED }, // Bottom row
},
{
    {   0,   0 }, {   24,   0 },{   48,   0 },{   72,   0 },{   96,   0 },{   120,   0 },{   144,   0 },{   168,   0 }, {   192,   0 },{   216,   0 },
    {   216, 13 },{   192,  13 },{   168, 13 },{   144, 13 },{   120, 13 },{   96,   13 },{   72,   13 },{   48,   13 },{   24,   13 },{   0,   13 },
    {   0,   26 }, {   24,  26 },{   48,  26 },{   72,  26 },{   96,  26 },{   120,  26 },{   144,  26 },{   168,  26 },{   192,  26 },{   216, 26 },
    {   168,  39 },{   144,   39 },{   120,  39 },{   96,   39},{   72,   39 },{   48,   39 }

},
{
    /* LED index to flag. */
     LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER,LED_FLAG_MODIFIER,
     LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
     LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
     LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER,LED_FLAG_MODIFIER,
     LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER,LED_FLAG_MODIFIER,
     LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
     LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
     LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER
} };
#endif
// clang-format on


