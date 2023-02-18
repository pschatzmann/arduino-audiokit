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

#if __has_include("kit_custom_pins.h")
# include "kit_custom_pins.h"
#endif

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
#  define SDCARD_INTR_GPIO          GPIO_NUM_34
#endif

#ifndef SDCARD_PWR_CTRL
#  define SDCARD_PWR_CTRL           GPIO_NUM_13
#endif


// I2S
#ifndef PIN_I2S_AUDIO_KIT_MCLK
#  define PIN_I2S_AUDIO_KIT_MCLK 0
#endif
#ifndef PIN_I2S_AUDIO_KIT_BCK
#  define PIN_I2S_AUDIO_KIT_BCK GPIO_NUM_5
#endif
#ifndef PIN_I2S_AUDIO_KIT_WS
#  define PIN_I2S_AUDIO_KIT_WS GPIO_NUM_25
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_OUT
#  define PIN_I2S_AUDIO_KIT_DATA_OUT GPIO_NUM_26
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_IN
#  define PIN_I2S_AUDIO_KIT_DATA_IN GPIO_NUM_35
#endif

#ifndef PIN_I2S_AUDIO_KIT_BCK1
#  define PIN_I2S_AUDIO_KIT_BCK1 GPIO_NUM_32
#endif
#ifndef PIN_I2S_AUDIO_KIT_WS1
#  define PIN_I2S_AUDIO_KIT_WS1 GPIO_NUM_33
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_OUT1
#  define PIN_I2S_AUDIO_KIT_DATA_OUT1 -1
#endif
#ifndef PIN_I2S_AUDIO_KIT_DATA_IN1
#  define PIN_I2S_AUDIO_KIT_DATA_IN1 GPIO_NUM_36
#endif


// I2C
#ifndef I2C_MASTER_NUM
#  define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#endif
#ifndef I2C_MASTER_SCL_IO
#  define I2C_MASTER_SCL_IO GPIO_NUM_23     
#endif
#ifndef I2C_MASTER_SDA_IO
#  define I2C_MASTER_SDA_IO GPIO_NUM_18    
#endif
#ifndef I2C_MASTER_ADDR
#  define I2C_MASTER_ADDR 0x10
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

#define PIN_KEY1 BUTTON_REC_ID
#define PIN_KEY2 BUTTON_MODE_ID
#define PIN_KEY3 BUTTON_SET_ID
#define PIN_KEY4 BUTTON_PLAY_ID
#define PIN_KEY5 BUTTON_VOLDOWN_ID
#define PIN_KEY6 BUTTON_VOLUP_ID


#define ES8311_MCLK_SOURCE        0   /* 0 From MCLK of esp32   1 From BCLK */

#ifndef HEADPHONE_DETECT
#  define HEADPHONE_DETECT          GPIO_NUM_19
#endif

#ifndef PA_ENABLE_GPIO
#  define PA_ENABLE_GPIO            GPIO_NUM_21
#endif

#ifndef BLUE_LED_GPIO
#  define BLUE_LED_GPIO             GPIO_NUM_27
#endif

#ifndef GREEN_LED_GPIO
#  define GREEN_LED_GPIO            GPIO_NUM_22
#endif

extern audio_hal_func_t AUDIO_CODEC_ES8311_DEFAULT_HANDLE;
extern audio_hal_func_t AUDIO_CODEC_ES7243_DEFAULT_HANDLE;

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

#endif
