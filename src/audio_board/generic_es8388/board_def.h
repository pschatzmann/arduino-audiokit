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

#ifndef _AUDIO_BOARD_DEFINITION_H_
#define _AUDIO_BOARD_DEFINITION_H_

#include "audio_hal/audio_hal.h"

#define ES8388

// SD Card
#ifndef PIN_AUDIO_KIT_SD_CARD_CS
#  define PIN_AUDIO_KIT_SD_CARD_CS 13
#endif
#ifndef PIN_AUDIO_KIT_SD_CARD_MISO
#  define PIN_AUDIO_KIT_SD_CARD_MISO 2
#endif
#ifndef PIN_AUDIO_KIT_SD_CARD_MOSI
#  define PIN_AUDIO_KIT_SD_CARD_MOSI 15
#endif
#ifndef PIN_AUDIO_KIT_SD_CARD_CLK
#  define PIN_AUDIO_KIT_SD_CARD_CLK  14
#endif

// I2S
#ifndef PIN_I2S_AUDIO_KIT_MCLK
#  define PIN_I2S_AUDIO_KIT_MCLK 0
#endif
#ifndef PIN_I2S_AUDIO_KIT_BCK
#  define PIN_I2S_AUDIO_KIT_BCK 27
#endif
#ifndef PIN_I2S_AUDIO_KIT_WS
#  define PIN_I2S_AUDIO_KIT_WS 25
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_OUT
#  define PIN_I2S_AUDIO_KIT_DATA_OUT 26
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_IN
#  define PIN_I2S_AUDIO_KIT_DATA_IN 35
#endif

// I2C
#ifndef I2C_MASTER_NUM
#  define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#endif
#ifndef I2C_MASTER_SCL_IO
#  define I2C_MASTER_SCL_IO 32     
#endif
#ifndef I2C_MASTER_SDA_IO
#  define I2C_MASTER_SDA_IO 33    
#endif
#ifndef I2C_MASTER_ADDR
#  define I2C_MASTER_ADDR 0x10
#endif


#ifndef SDCARD_OPEN_FILE_NUM_MAX
#  define SDCARD_OPEN_FILE_NUM_MAX  5
#endif
#ifndef SDCARD_INTR_GPIO
#  define SDCARD_INTR_GPIO          GPIO_NUM_34
#endif

#ifndef PIN_KEY1
#  define PIN_KEY1 36
#endif
#ifndef PIN_KEY2
#  define PIN_KEY2 13
#endif
#ifndef PIN_KEY3
#  define PIN_KEY3 19
#endif
#ifndef PIN_KEY4
#  define PIN_KEY4 23
#endif
#ifndef PIN_KEY5
#  define PIN_KEY5 18
#endif
#ifndef PIN_KEY6
#  define PIN_KEY6 5
#endif

#define BUTTON_REC_ID             PIN_KEY1
#define BUTTON_MODE_ID            PIN_KEY2
#define BUTTON_SET_ID             PIN_KEY3
#define BUTTON_PLAY_ID            PIN_KEY4
#define BUTTON_VOLDOWN_ID         PIN_KEY5
#define BUTTON_VOLUP_ID           PIN_KEY6


#ifndef AUXIN_DETECT_GPIO
#  define AUXIN_DETECT_GPIO         GPIO_NUM_12
#endif
#ifndef HEADPHONE_DETECT
#  define HEADPHONE_DETECT          GPIO_NUM_39
#endif
#ifndef PA_ENABLE_GPIO
#  define PA_ENABLE_GPIO            GPIO_NUM_21
#endif


#ifndef GREEN_LED_GPIO
#  ifndef GPIO_NUM_22
#    define GPIO_NUM_22              -1
#  endif
#  define GREEN_LED_GPIO            GPIO_NUM_22
#endif

extern audio_hal_func_t AUDIO_CODEC_ES8388_DEFAULT_HANDLE;

#define AUDIO_CODEC_DEFAULT_CONFIG(){                   \
        .adc_input  = AUDIO_HAL_ADC_INPUT_LINE1,        \
        .dac_output = AUDIO_HAL_DAC_OUTPUT_ALL,         \
        .codec_mode = AUDIO_HAL_CODEC_MODE_BOTH,        \
        .i2s_iface = {                                  \
            .mode = AUDIO_HAL_MODE_SLAVE,               \
            .fmt = AUDIO_HAL_I2S_NORMAL,                \
            .samples = AUDIO_HAL_48K_SAMPLES,           \
            .bits = AUDIO_HAL_BIT_LENGTH_16BITS,        \
        },                                              \
};

#ifndef INPUT_KEY_NUM
#  define   INPUT_KEY_NUM     6
#endif

#define INPUT_KEY_DEFAULT_INFO() {                      \
     {                                                  \
        .type = PERIPH_ID_BUTTON,                       \
        .user_id = INPUT_KEY_USER_ID_REC,               \
        .act_id = BUTTON_REC_ID,                        \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_BUTTON,                       \
        .user_id = INPUT_KEY_USER_ID_MODE,              \
        .act_id = BUTTON_MODE_ID,                       \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_TOUCH,                        \
        .user_id = INPUT_KEY_USER_ID_SET,               \
        .act_id = BUTTON_SET_ID,                        \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_TOUCH,                        \
        .user_id = INPUT_KEY_USER_ID_PLAY,              \
        .act_id = BUTTON_PLAY_ID,                       \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_TOUCH,                        \
        .user_id = INPUT_KEY_USER_ID_VOLUP,             \
        .act_id = BUTTON_VOLUP_ID,                      \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_TOUCH,                        \
        .user_id = INPUT_KEY_USER_ID_VOLDOWN,           \
        .act_id = BUTTON_VOLDOWN_ID,                    \
    }                                                   \
}

#endif
