/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#ifndef _SDCARD_DEV_H_
#define _SDCARD_DEV_H_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief  SD card mode, SPI, 1-line SD mode, 4-line SD mode
 *
 */
typedef enum {
    SD_MODE_SPI    = 0x0,  /*!< sd_card SPI*/
    SD_MODE_1_LINE = 0x1,  /*!< sd_card 1-line SD mode*/
    SD_MODE_4_LINE = 0x2,  /*!< sd_card 4-line SD mode*/
    SD_MODE_MAX,
} periph_sdcard_mode_t;

/**
 * @brief   The SD Card Peripheral configuration
 */
typedef struct {
    int card_detect_pin;        /*!< Card detect gpio number */
    const char* root;           /*!< Base path for vfs */
    periph_sdcard_mode_t mode;  /*!< card mode*/
} periph_sdcard_cfg_t;


/**
 * @brief DUMMY Definition - not used
 * 
 */

typedef void* esp_periph_handle_t;

/// Just setup the SPI bus
esp_periph_handle_t periph_sdcard_init(periph_sdcard_cfg_t* sdcard_config);


#ifdef __cplusplus
}
#endif

#endif
