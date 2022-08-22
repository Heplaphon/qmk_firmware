#include "hotdox76v2.h"
#include <string.h>
#include <transactions.h>
// #include "oled_font_lib/logo.h"
#include "oled_font_lib/logo2.h"
#include "oled_font_lib/ext_font.h"




bool is_keyboard_left(void) {

#ifdef I_AM_LEFT
    return true;
#else
    return false;
#endif

}


#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {
    {
        { NO_LED, 34, 33, 32, 35, 37, 36     },
        { 27, 28, 29, 30, 31, NO_LED, NO_LED },
        { 26, 25, 24, 23, 22, 21, 20         },
        { 14, 15, 16, 17, 18, 19, NO_LED     },
        { 13, 12, 11, 10,  9,  8,  7         },
        {   0, 1,  2,   3, 4,  5,  6         },
        /*right*/
        { NO_LED, 72, 71, 70, 73, 75, 74     },
        { 65, 66, 67, 68, 69, NO_LED, NO_LED },
        { 64, 63, 62, 61, 60, 59, 58         },
        { 52, 53, 54, 55, 56, 57, NO_LED     },
        { 51, 50, 49, 48, 47, 46, 45         },
        { 38, 39, 40, 41, 42, 43, 44         }
    },
    {
        // LED Index to Physical Position
        {0,0},    {17,0},  {34,0},  {52,0},  {69,0},  {86,0},  {103,0}, \
        {103,13}, {86,13}, {69,13}, {52,13}, {34,13}, {17,13}, {0,13}, \
        {0,26},   {17,26}, {34,26}, {52,26}, {69,26}, {86,26}, \
        {103,38}, {86,38}, {69,38}, {52,38}, {34,38}, {17,38}, {0,38}, \
        {0,51},   {17,51}, {34,51}, {52,51}, {69,51},  \
                  {86,64}, {69,64}, {52,64}, {34,64}, {17,64}, {0,64}, \
                  {69,64}, {52,64}, {34,64}, {17,64}, {0,64}, \

                                                                                    \
        {224,0},  {207,0},  {190,0},  {172,0},  {155,0},  {138,0},  {121,0}, \
        {121,13}, {138,13}, {155,13}, {172,13}, {190,13}, {207,13}, {224,13}, \
        {224,26}, {207,26}, {190,26}, {172,26}, {155,26}, {138,26}, \
        {121,38}, {138,38}, {155,38}, {172,38}, {190,38}, {207,38}, {224,38}, \
        {224,51}, {207,51}, {190,51}, {172,51}, {155,51},  \
                  {138,64}, {155,64}, {172,64}, {190,64}, {207,64}, {224,64}, \
                  {155,64}, {172,64}, {190,64}, {207,64}, {224,64}\


    },
    {
      // LED Index to Flag
      4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,\
      2,2,2,2,2,\
      4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,\
      2,2,2,2,2
    }
};


#endif



#ifdef OLED_ENABLE

#   define UNC (94+0x21)
typedef struct _master_to_slave_t {
    int cur_alp_index;
    char current_alp[7];
} master_to_slave_t;
master_to_slave_t m2s;

typedef struct _slave_to_master_t {
    int cur_alp_index;
    char current_alp[7];
} slave_to_master_t;
slave_to_master_t s2m;

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    strcpy((char *)(m2s.current_alp), "[    ]");
    m2s.current_alp[1] = UNC;
    m2s.current_alp[2] = UNC;
    m2s.current_alp[3] = UNC;
    m2s.current_alp[4] = UNC;

    m2s.cur_alp_index = 1;

    strcpy((char *)(s2m.current_alp), "[    ]");
    s2m.current_alp[1] = UNC;
    s2m.current_alp[2] = UNC;
    s2m.current_alp[3] = UNC;
    s2m.current_alp[4] = UNC;

    s2m.cur_alp_index = 1;

#   ifdef I_AM_LEFT
        return OLED_ROTATION_180;
#   else
        return OLED_ROTATION_0;
#   endif
}


void render_logo(void) {
    uint8_t i = 0, j = 0;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 32; ++j) {
#   ifdef I_AM_LEFT
            oled_write_raw_byte(pgm_read_byte(&logo_mouse[i*32+j]), i*128+j);
// else
            //oled_write_raw_byte(pgm_read_byte(&logo_mouse[i*32+j]), i*128+j+96);
#   endif
        }
    }
}

#   ifdef I_AM_LEFT
void render_layer_helper_fun(uint8_t start_line, const char * data, uint8_t gap_w, uint8_t l) {
    uint8_t j = 0, k = 0;
    for (j = 0; j < l; ++j) { // font index
        for (k = 0; k < 12; ++k) { // font byte index
            //                                        base + logo_w(32) + gap_w(12) +l*font_w(12)+current_byte_index
            oled_write_raw_byte(pgm_read_byte(&ext_big_font[pgm_read_byte(&data[j])-0x21][k]), start_line*2*128 + 32 + gap_w + j*12+k);
            oled_write_raw_byte(pgm_read_byte(&ext_big_font[pgm_read_byte(&data[j])-0x21][k+12]), start_line*2*128+128 + 32 + gap_w + j*12+k);
        }
    }
    for (j = 0; j < gap_w; ++j) {
        oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128 + 32 + j);
        oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128 + 32 + gap_w + l*12 + j);

        oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128+128 + 32 + j);
        oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128+128 + 32 + gap_w + l*12 + j);

    }
}
void render_layer(uint8_t layer) {
    render_layer_helper_fun(0, PSTR("LAYER:"), 12, 6);
    switch (layer) {
    case 0:
        render_layer_helper_fun(1, PSTR("1:HOME"), 12, 6);
        break;
    case 1:
        render_layer_helper_fun(1, PSTR("2:CODE"), 12, 6);
        break;
    case 2:
        render_layer_helper_fun(1, PSTR("3:OFFICE"), 0, 8);
        break;
    case 3:
    default:
        render_layer_helper_fun(1, PSTR("4:OTHERS"), 0, 8);
        break;
    }
}

# endif

#   ifndef I_AM_LEFT

// void render_cur_input_helper_fun(uint8_t start_line, const char * data, uint8_t gap_w, uint8_t l) {
//     uint8_t j = 0, k = 0;
//     for (j = 0; j < l; ++j) { // font index
//         for (k = 0; k < 12; ++k) { // font byte index
//             //                                        base + logo_w(0) + gap_w(12) +l*font_w(12)+current_byte_index
//             oled_write_raw_byte(pgm_read_byte(&ext_big_font[data[j]-0x21][k]), start_line*2*128 + gap_w + j*12+k);
//             oled_write_raw_byte(pgm_read_byte(&ext_big_font[data[j]-0x21][12+k]), start_line*2*128+128 + gap_w + j*12+k);
//         }
//     }
//     for (j = 0; j < gap_w; ++j) {
//         oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128 + j);
//         oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128 + gap_w + l*12 + j);
// 
//         oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128+128 + j);
//         oled_write_raw_byte(pgm_read_byte(&blank_block), start_line*2*128+128 + gap_w + l*12 + j);
//     }
// }
// 
// void render_cur_input(void) {
//     render_cur_input_helper_fun(0, "INPUTS:", 6, 7);
//     if (is_keyboard_master()) {
//         render_cur_input_helper_fun(1, (const char *)(m2s.current_alp), 12, 6);
//     } else {
//         render_cur_input_helper_fun(1, (const char *)(s2m.current_alp), 12, 6);
//     }
//     return;
// }
char wpm_str[32];

// WPM-responsive animation stuff here
#define IDLE_FRAMES 1
#define IDLE_SPEED 25 // below this wpm value your animation will idle

// #define PREP_FRAMES 1 // uncomment if >1

#define TAP_FRAMES 2
#define TAP_SPEED 30 // above this wpm value typing animation to triggere

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#define ANIM_SIZE 512 // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;
// uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
uint8_t current_tap_frame = 0;

// Images credit j-inc(/James Incandenza) and pixelbenny. Credit to obosob for initial animation approach.
static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
        0,  0,  0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
        },
//        {
//        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
//        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
//        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
//        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
//        },
//        {
//        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16,  8,  4,  2,  2,  4, 24, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
//        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60,194,  1,  1,  2,  2,  4,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 96, 96,  0,129,130,130,132,  8, 16, 32, 64,128,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
//        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 25,  6,  0,  0,  0,  0,  0,  0,  0, 24, 24, 24, 27,  3,  0, 64,160, 34, 36, 20, 18, 18, 18, 11,  8,  8,  8,  8,  5,  5,  9,  9, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
//        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
//        },
//        {
//        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
//        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
//        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
//        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
//        },
//        {
//        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  4,  2,  2,  2,  4, 56, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
//        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28,226,  1,  1,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
//        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
//        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
//        }
    };
    static const char PROGMEM prep[][ANIM_SIZE] = {
        {
        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132,195,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
        }
    };
    static const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE] = {
        {
        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,248,248,248,248,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,  1,  2,  4,  8, 16, 32, 67,135,  7,  1,  0,184,188,190,159, 95, 95, 79, 76, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132, 67,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1, 61,124,252,252,252,252,252, 60, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32,  16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
        },
        {
        0,  0,0,0, 0, 0,0, 0,  0, 0, 0,0,0, 0, 0,  0,0,0, 0, 0,0,0,  0,0,0,  0,0,0, 0, 0,0,0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
        0,  0,  0,  0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,0,0,0,0, 0, 0, 0, 0,0,0, 0, 0, 0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
        0,  0,0, 0,  0,  0,0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,0,  0,  0, 0,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
        0,  0, 0,0,0,  0, 0, 0,0,0,0, 0, 0, 0, 0,0,  0,0,  0,0, 0, 128, 128, 128, 128,64,64, 64, 64,  32, 32, 32, 32,  16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,122,122,121,121,121,121, 57, 49,  2,  2,  4,  4,  8,  8,  8,8,8,7,0,  0,  0,  48,  60,  124,  124,  126,  126,  126,  126,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,2,7, 31,  7,31,  7, 28,  7,31,  7,31,  7,  2, 2,  0,  0,  0,  0,
        },
    };

    //assumes 1 frame prep stage
    void animation_phase(void) {
        if(get_current_wpm() <=IDLE_SPEED){
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
         }
         if(get_current_wpm() >IDLE_SPEED && get_current_wpm() <TAP_SPEED){
             // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
             oled_write_raw_P(prep[0], ANIM_SIZE);  // remove if IDLE_FRAMES >1
         }
         if(get_current_wpm() >=TAP_SPEED){
             current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
             oled_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], ANIM_SIZE);
         }
    }
    if(get_current_wpm() != 000) {
        oled_on(); // not essential but turns on animation OLED with any alpha keypress
        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {
        if(timer_elapsed32(anim_sleep) <= OLED_TIMEOUT) {
            if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}
#endif


bool oled_task_kb(void) {
    render_logo();
#   ifdef I_AM_LEFT
    render_layer(biton32(layer_state));
#   else
    // render_cur_input();
    render_anim();
	oled_set_cursor(0,0);
    //sprintf(wpm_str, "WPM:%03d", get_current_wpm());
    oled_write(get_u8_str(get_current_wpm(), '0'), false);
    // oled_write(wpm_str, false);
#   endif
    return false;
}



// static const char PROGMEM code_to_name[0xFF] = {
// //   0    1    2    3    4    5    6    7    8    9    A    B    c    D    E    F
//     UNC,        UNC,        UNC,        UNC,        'a',        'b',        'c',        'd',        'e',        'f',        'g',        'h',        'i',        'j',        'k',        'l',         // 0x
//     'm',        'n',        'o',        'p',        'q',        'r',        's',        't',        'u',        'v',        'w',        'x',        'y',        'z',        '1',        '2',         // 1x
//     '3',        '4',        '5',        '6',        '7',        '8',        '9',        '0',        UNC,        UNC,        UNC,        UNC,        UNC,        '-',        '=',        '[',         // 2x
//     ']',       '\\',        '#',        ';',       '\'',        '`',        ',',        '.',        '/',        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // 3x
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // 4x
//     UNC,        UNC,        UNC,        UNC,        '/',        '*',        '-',        '+',        UNC,        '1',        '2',        '3',        '4',        '5',        '6',        '7',         // 5x
//     '8',        '9',        '0',        '.',        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // 6x
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // 7x
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // 8x
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // 9x
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // Ax
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // Bx
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // Cx
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // Dx
//     UNC,        UNC,        'A',        'W',        UNC,        'S',        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,         // Ex
//     UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC,        UNC        // Fx
// };
// 
// 
// 
// 
// void get_cur_alp_hook(uint16_t keycode) {
//     if (keycode >= 0xF0) {
//         keycode = 0xF0;
//     }
//     if (m2s.cur_alp_index < 4) {
//         m2s.current_alp[m2s.cur_alp_index] = pgm_read_byte(&code_to_name[keycode]);
//         if (m2s.cur_alp_index == 1) {
//             m2s.current_alp[2] = m2s.current_alp[3] = m2s.current_alp[4]  = UNC;
//         }
//         m2s.cur_alp_index++;
//     } else {
//         for (uint8_t i = 2; i <= 4; ++i) {
//             m2s.current_alp[i-1] = m2s.current_alp[i];
//         }
//         m2s.current_alp[m2s.cur_alp_index] = pgm_read_byte(&code_to_name[keycode]);
//     }
// }

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
//    if (record->event.pressed) {
//        get_cur_alp_hook(keycode);
//    }
    switch(keycode) {
        case TOG_OLED:
            if (record->event.pressed) {
                if (is_oled_on()) {
                    oled_off();
                } else {
                    oled_on();
                }
            }
            return false;
        default:
            return true;
    }
    return true;

}


void matrix_scan_kb(void) {
    if(!is_oled_on()) {
        m2s.cur_alp_index = 1;
    }
}



void user_sync_alpa_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const master_to_slave_t *m2s_p = (const master_to_slave_t*)in_data;
    s2m.cur_alp_index = m2s_p->cur_alp_index;
    for (size_t i = 0; i < 7; i++)
    {
        s2m.current_alp[i] = m2s_p->current_alp[i];
    }
}

void keyboard_post_init_kb(void) {
    transaction_register_rpc(KEYBOARD_CURRENT_ALPA_SYNC, user_sync_alpa_slave_handler);
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        // Interact with slave every 200ms
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 200) {
            if(transaction_rpc_exec(KEYBOARD_CURRENT_ALPA_SYNC, sizeof(m2s), &m2s, sizeof(s2m), &s2m)) {
                last_sync = timer_read32();
                dprint("Slave sync successed!\n");
            } else {
                dprint("Slave sync failed!\n");
            }
        }
    }
}


#endif



// void keyboard_post_init_kb(void) {
//     // eeconfig_init();
//     // Customise these values to desired behaviour
//     // debug_enable=true;
//     // debug_matrix=false;
//     // debug_keyboard=true;
//     //debug_mouse=true;
// }
