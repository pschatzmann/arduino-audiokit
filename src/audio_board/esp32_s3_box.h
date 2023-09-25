/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2022 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
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

// for AUDIOKIT_BOARD==8

#pragma once

#define AUDIO_DRIVER AUDIO_CODEC_ES8311_DEFAULT_HANDLE
#define AUDIO_DRIVER_ADC AUDIO_CODEC_ES7210_DEFAULT_HANDLE

/**
 * @brief LCD SCREEN Function Definition
 */
#define FUNC_LCD_SCREEN_EN          (1)
#define LCD_CTRL_GPIO               45
#define LCD_RST_GPIO                48
#define LCD_DC_GPIO                 4
#define LCD_CS_GPIO                 5
#define LCD_CLK_GPIO                7
#define LCD_MOSI_GPIO               6
// The LCD pixel number in horizontal and vertical
#define LCD_H_RES                   320
#define LCD_V_RES                   240
#define LCD_SWAP_XY                 (false)
#define LCD_MIRROR_X                (true)
#define LCD_MIRROR_Y                (true)
#define LCD_COLOR_INV               (false)
#define LCD_COLOR_SPACE             ESP_LCD_COLOR_SPACE_BGR


/**
 * @brief SDCARD Function Definition
 *        PMOD2 for one line sdcard
 */
#define SDCARD_OPEN_FILE_NUM_MAX    5
#define SDCARD_INTR_GPIO            -1
#define SDCARD_PWR_CTRL             -1
#define ESP_SD_PIN_CLK              13
#define ESP_SD_PIN_CMD              11
#define ESP_SD_PIN_D0               14
#define ESP_SD_PIN_D1               -1
#define ESP_SD_PIN_D2               -1
#define ESP_SD_PIN_D3               -1
#define ESP_SD_PIN_D4               -1
#define ESP_SD_PIN_D5               -1
#define ESP_SD_PIN_D6               -1
#define ESP_SD_PIN_D7               -1
#define ESP_SD_PIN_CD               -1
#define ESP_SD_PIN_WP               -1

#define PIN_AUDIO_KIT_SD_CARD_CS -1
#define PIN_AUDIO_KIT_SD_CARD_MISO 14
#define PIN_AUDIO_KIT_SD_CARD_MOSI 11
#define PIN_AUDIO_KIT_SD_CARD_CLK  13

// I2S
#define PIN_I2S_AUDIO_KIT_MCLK 2
#define PIN_I2S_AUDIO_KIT_BCK 17
#define PIN_I2S_AUDIO_KIT_WS 47
#define PIN_I2S_AUDIO_KIT_DATA_OUT 15
#define PIN_I2S_AUDIO_KIT_DATA_IN 16

#define PIN_I2S_AUDIO_KIT_MCLK1 2
#define PIN_I2S_AUDIO_KIT_BCK1 17
#define PIN_I2S_AUDIO_KIT_WS1 47
#define PIN_I2S_AUDIO_KIT_DATA_OUT1 15
#define PIN_I2S_AUDIO_KIT_DATA_IN1 16

// I2C
#define I2C_MASTER_NUM I2C_NUM_0 /*!< I2C port number for master dev */
#define I2C_MASTER_SCL_IO 18     
#define I2C_MASTER_SDA_IO 8    


/**
 * @brief LCD TOUCH PANEL Function Definition
 */
#define FUNC_LCD_TOUCH_EN          (1)
#define TOUCH_PANEL_SWAP_XY        (0)
#define TOUCH_PANEL_INVERSE_X      (1)
#define TOUCH_PANEL_INVERSE_Y      (0)

#define GREEN_LED_GPIO             -1
#define BLUE_LED_GPIO              -1
#define ADC_DETECT                 -1
#define ES7243_MCLK                -1
#define RESET_CODEC                -1
#define RESET_BOARD                -1
#define AUXIN_DETECT_GPIO          -1
#define PIN_KEY1                   -1
#define PIN_KEY2                   -1
#define PIN_KEY3                   -1
#define PIN_KEY4                   -1
#define PIN_KEY5                   -1
#define PIN_KEY6                   -1

/**
 * @brief Audio Codec Chip Function Definition
 */
#define FUNC_AUDIO_CODEC_EN         (1)
#define PA_ENABLE_GPIO              46
#define HEADPHONE_DETECT            -1
#define CODEC_ADC_I2S_PORT          (0)
#define CODEC_ADC_BITS_PER_SAMPLE   I2S_BITS_PER_SAMPLE_32BIT
#define CODEC_ADC_SAMPLE_RATE       (48000)
#define RECORD_HARDWARE_AEC         (true)
#define BOARD_PA_GAIN               (0) /* Power amplifier gain defined by board (dB) */


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



#define INPUT_KEY_DEFAULT_INFO() {                      \
    {                                                   \
        .type = PERIPH_ID_ADC_BTN,                      \
        .user_id = INPUT_KEY_USER_ID_SET,               \
        .act_id = BUTTON_SET_ID,                        \
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

