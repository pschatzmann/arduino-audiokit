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

// for AUDIOKIT_BOARD==11

#pragma once

#if __has_include("kit_custom_pins.h")
# include "kit_custom_pins.h"
#endif

#define AUDIO_DRIVER AUDIO_CODEC_ES8311_DEFAULT_HANDLE

// SD
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

#ifndef SDCARD_OPEN_FILE_NUM_MAX
#define SDCARD_OPEN_FILE_NUM_MAX  5
#endif

#ifndef SDCARD_INTR_GPIO
#  define SDCARD_INTR_GPIO          34
#endif

#ifndef SDCARD_PWR_CTRL
#  define SDCARD_PWR_CTRL           13
#endif


// I2S
#ifndef PIN_I2S_AUDIO_KIT_MCLK
#  define PIN_I2S_AUDIO_KIT_MCLK 0
#endif
#ifndef PIN_I2S_AUDIO_KIT_BCK
#  define PIN_I2S_AUDIO_KIT_BCK 5
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

#ifndef PIN_I2S_AUDIO_KIT_MCLK1
#  define PIN_I2S_AUDIO_KIT_MCLK1 0
#endif
#ifndef PIN_I2S_AUDIO_KIT_BCK1
#  define PIN_I2S_AUDIO_KIT_BCK1 32
#endif
#ifndef PIN_I2S_AUDIO_KIT_WS1
#  define PIN_I2S_AUDIO_KIT_WS1 33
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_OUT1
#  define PIN_I2S_AUDIO_KIT_DATA_OUT1 -1
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_IN1
#  define PIN_I2S_AUDIO_KIT_DATA_IN1 36
#endif


// I2C
#ifndef I2C_MASTER_NUM
#  define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#endif
#ifndef I2C_MASTER_SCL_IO
#  define I2C_MASTER_SCL_IO 23     
#endif
#ifndef I2C_MASTER_SDA_IO
#  define I2C_MASTER_SDA_IO 18    
#endif

#ifndef ES7243_MCLK_GPIO
#define ES7243_MCLK_GPIO          PIN_I2S_AUDIO_KIT_MCLK
#endif

#ifndef BUTTON_VOLUP_ID
#  define BUTTON_VOLUP_ID           0
#endif

#ifndef BUTTON_VOLDOWN_ID
#  define BUTTON_VOLDOWN_ID         1
#endif

#ifndef BUTTON_SET_ID
#  define BUTTON_SET_ID             2
#endif

#ifndef BUTTON_PLAY_ID
#  define BUTTON_PLAY_ID            3
#endif

#ifndef BUTTON_MODE_ID
#  define BUTTON_MODE_ID            4
#endif

#ifndef BUTTON_REC_ID
#  define BUTTON_REC_ID             5
#endif

#define ES8311_MCLK_SOURCE        0   /* 0 From MCLK of esp32   1 From BCLK */

#ifndef HEADPHONE_DETECT
#  define HEADPHONE_DETECT          19
#endif

#ifndef PA_ENABLE_GPIO
#  define PA_ENABLE_GPIO            21
#endif

#ifndef BLUE_LED_GPIO
#  define BLUE_LED_GPIO             27
#endif

#ifndef GREEN_LED_GPIO
#  define GREEN_LED_GPIO            22
#endif

#define ADC_DETECT                 -1
#define ES7243_MCLK                -1
#define RESET_CODEC                -1
#define RESET_BOARD                -1
#define BLUE_LED_GPIO              -1
#define AUXIN_DETECT_GPIO          -1


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


#define INPUT_KEY_NUM     6

#define INPUT_KEY_DEFAULT_INFO() {                      \
     {                                                  \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_REC,               \
        .act_id = BUTTON_REC_ID,                        \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_MODE,              \
        .act_id = BUTTON_MODE_ID,                       \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_SET,               \
        .act_id = BUTTON_SET_ID,                        \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_PLAY,              \
        .act_id = BUTTON_PLAY_ID,                       \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_VOLUP,             \
        .act_id = BUTTON_VOLUP_ID,                      \
    },                                                  \
    {                                                   \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_VOLDOWN,           \
        .act_id = BUTTON_VOLDOWN_ID,                    \
    }                                                   \
}