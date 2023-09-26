/*
 * @Author: your name
 * @Date: 2020-01-26 10:13:06
 * @LastEditTime : 2020-01-28 19:24:42
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \esp-adf\components\audio_board\aithinker\board_def.h
 */
/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

// for AUDIOKIT_BOARD==6

#pragma once

#define AUDIO_DRIVER AUDIO_CODEC_AC101_CODEC_HANDLE

// SD Card Pins
#include "common/esp32_sd.h"

#define SDCARD_INTR_GPIO 34
#define SDCARD_OPEN_FILE_NUM_MAX  5

// I2S
#define PIN_I2S_AUDIO_KIT_MCLK 0
#define PIN_I2S_AUDIO_KIT_BCK 27
#define PIN_I2S_AUDIO_KIT_WS 26
#define PIN_I2S_AUDIO_KIT_DATA_OUT 25
#define PIN_I2S_AUDIO_KIT_DATA_IN 35
#define PIN_I2S_AUDIO_KIT_MCLK1 0
#define PIN_I2S_AUDIO_KIT_BCK1 27
#define PIN_I2S_AUDIO_KIT_WS1 26
#define PIN_I2S_AUDIO_KIT_DATA_OUT1 25
#define PIN_I2S_AUDIO_KIT_DATA_IN1 35

// I2C
#define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_SCL_IO 32     
#define I2C_MASTER_SDA_IO 33    


#define HEADPHONE_DETECT 5
#define PA_ENABLE_GPIO 21

#define GREEN_LED_GPIO 22
#define BLUE_LED_GPIO 19

/* KEY1 tp 6 */
#define PIN_KEY1 36
#define PIN_KEY2 13
#define PIN_KEY3 19
#define PIN_KEY4 23
#define PIN_KEY5 18
#define PIN_KEY6 5

#define BUTTON_SET 19
#define BUTTON_PLAY 23
#define BUTTON_VOLUP 18
#define BUTTON_VOLDWN 5

#define AUXIN_DETECT_GPIO          -1
#define ADC_DETECT                 -1
#define ES7243_MCLK                -1
#define RESET_CODEC                -1
#define RESET_BOARD                -1
#define SDCARD_INTR_GPIO           -1

#define AUDIO_CODEC_DEFAULT_CONFIG() {       \
    .adc_input = AUDIO_HAL_ADC_INPUT_LINE1,  \
    .dac_output = AUDIO_HAL_DAC_OUTPUT_ALL,  \
    .codec_mode = AUDIO_HAL_CODEC_MODE_BOTH, \
    .i2s_iface = {                           \
        .mode = AUDIO_HAL_MODE_SLAVE,        \
        .fmt = AUDIO_HAL_I2S_NORMAL,         \
        .samples = AUDIO_HAL_48K_SAMPLES,    \
        .bits = AUDIO_HAL_BIT_LENGTH_16BITS, \
    },                                       \
};

#define INPUT_KEY_NUM     6

#define INPUT_KEY_DEFAULT_INFO() {                      \
     {                                                  \
        .type = PERIPH_ID_BUTTON,                       \
        .user_id = INPUT_KEY_USER_ID_REC,               \
        .act_id = PIN_KEY1,                        \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_BUTTON,                       \
        .user_id = INPUT_KEY_USER_ID_MODE,              \
        .act_id = PIN_KEY2,                       \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_BUTTON,                        \
        .user_id = INPUT_KEY_USER_ID_SET,               \
        .act_id = PIN_KEY3,                        \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_BUTTON,                        \
        .user_id = INPUT_KEY_USER_ID_PLAY,              \
        .act_id = PIN_KEY4,                       \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_BUTTON,                        \
        .user_id = INPUT_KEY_USER_ID_VOLUP,             \
        .act_id = PIN_KEY5,                      \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_BUTTON,                        \
        .user_id = INPUT_KEY_USER_ID_VOLDOWN,           \
        .act_id = PIN_KEY6,                    \
    }                                                   \
}
