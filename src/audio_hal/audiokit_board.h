/**
 * @file audiokit_board.h
 * @author Phil Schatzmann
 * @brief Board driver which provides the pin information
 * @date 2023-5-25
 * 
 * @copyright Copyright (c) 2021
 **/

#pragma once

#ifdef ESP32
#include "hal/spi_types.h"
#include "driver/spi_slave.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"
#endif

#include <string.h>
#include "AudioKitSettings.h"
#include "audio_hal/audio_error.h"
#include "audio_hal/audio_gpio.h"
#include "audio_hal/audio_hal.h" // for audio_hal_func_t
#include "audio_hal/audio_mem.h"
#include "audio_hal/audiokit_board_select.h"
#include "audio_hal/audiokit_logger.h"


#ifndef ES8311_MCLK_SRC
#  define ES8311_MCLK_SRC -1
#endif

#ifndef BUTTON_REC_ID
#  define BUTTON_REC_ID             PIN_KEY1
#endif

#ifndef BUTTON_MODE_ID
#  define BUTTON_MODE_ID            PIN_KEY2
#endif

#ifndef BUTTON_SET_ID
#  define BUTTON_SET_ID             PIN_KEY3
#endif

#ifndef BUTTON_PLAY_ID
#  define BUTTON_PLAY_ID            PIN_KEY4
#endif

#ifndef BUTTON_VOLDOWN_ID
#  define BUTTON_VOLDOWN_ID         PIN_KEY5
#endif

#ifndef BUTTON_VOLUP_ID
#  define BUTTON_VOLUP_ID           PIN_KEY6
#endif


#define AUDIO_CODEC_DEFAULT_CONFIG()                                                               \
    {                   \
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


/**
 * Struct with all pins
 */
struct audio_kit_pins {
    // SD Card
    int8_t sd_cs = PIN_AUDIO_KIT_SD_CARD_CS;
    int8_t sd_miso = PIN_AUDIO_KIT_SD_CARD_MISO;
    int8_t sd_mosi = PIN_AUDIO_KIT_SD_CARD_MOSI;
    int8_t sd_clk = PIN_AUDIO_KIT_SD_CARD_CLK;
    int8_t sd_intr = SDCARD_INTR_GPIO;
    int8_t sd_max_open_files = SDCARD_OPEN_FILE_NUM_MAX;

    // I2S
    int8_t i2s_num = 0;
    int8_t i2s_mclk = PIN_I2S_AUDIO_KIT_MCLK; // usually 0
    int8_t i2s_bck = PIN_I2S_AUDIO_KIT_BCK;
    int8_t i2s_ws = PIN_I2S_AUDIO_KIT_WS;
    int8_t i2s_out = PIN_I2S_AUDIO_KIT_DATA_OUT;
    int8_t i2s_in = PIN_I2S_AUDIO_KIT_DATA_IN;

    int8_t i2s1_mclk = PIN_I2S_AUDIO_KIT_MCLK1; // usually 0
    int8_t i2s1_bck = PIN_I2S_AUDIO_KIT_BCK1;
    int8_t i2s1_ws = PIN_I2S_AUDIO_KIT_WS1;
    int8_t i2s1_out = PIN_I2S_AUDIO_KIT_DATA_OUT1;
    int8_t i2s1_in = PIN_I2S_AUDIO_KIT_DATA_IN1;

    // I2C
    int8_t i2c_num = I2C_NUM_0;
    int8_t i2c_scl = I2C_MASTER_SCL_IO;
    int8_t i2c_sda = I2C_MASTER_SDA_IO;
    
    int8_t key1 = PIN_KEY1;
    int8_t key2 = PIN_KEY2;
    int8_t key3 = PIN_KEY3;
    int8_t key4 = PIN_KEY4;
    int8_t key5 = PIN_KEY5;
    int8_t key6 = PIN_KEY6;

    int8_t detect_auxin = AUXIN_DETECT_GPIO;
    int8_t detect_headphone = HEADPHONE_DETECT;
    int8_t enable_pa = PA_ENABLE_GPIO;

    int8_t led_green = GREEN_LED_GPIO;

    int8_t adc_detect = ADC_DETECT;
    int8_t es7243_mclk = ES7243_MCLK;
    int8_t es8311_mclk_src = ES8311_MCLK_SRC;
    int8_t reset_codec = RESET_CODEC;
    int8_t reset_board = RESET_BOARD;
    int8_t led_blue = BLUE_LED_GPIO;
};

/**
 * Class which provides the pin assingments using the conventions used by ADF
 */
class board_driver {
public:
    void setup(audio_kit_pins pins)
    {
        this->pins = pins;
    }

    esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t* i2c_config)
    {
        AUDIO_NULL_CHECK(TAG, i2c_config, return ESP_FAIL);
        if (port == I2C_NUM_0) {
            i2c_config->sda_io_num = pins.i2c_sda;
            i2c_config->scl_io_num = pins.i2c_scl;
#ifdef I2C_NUM_1
        } else if (port == I2C_NUM_1 ) {
            // we use the same pins like in 0
            i2c_config->sda_io_num = pins.i2c_sda;
            i2c_config->scl_io_num = pins.i2c_scl;
#endif
        } else {
            i2c_config->sda_io_num = -1;
            i2c_config->scl_io_num = -1;
            KIT_LOGE("i2c port %d is not supported", port);
            return ESP_FAIL;
        }

        KIT_LOGI("i2c port: %d", port);
        KIT_LOGI("i2c sda: %d", i2c_config->sda_io_num);
        KIT_LOGI("i2c scl: %d", i2c_config->scl_io_num);

        return ESP_OK;
    }

    esp_err_t get_i2s_pins(int port, i2s_pin_config_t* i2s_config)
    {
        AUDIO_NULL_CHECK(TAG, i2s_config, return ESP_FAIL);
        if (port == I2S_NUM_0) {
            i2s_config->bck_io_num = pins.i2s_bck;
            i2s_config->ws_io_num = pins.i2s_ws;
            i2s_config->data_out_num = pins.i2s_out;
            i2s_config->data_in_num = pins.i2s_in;
            i2s_config->mck_io_num = pins.i2s_mclk;
#ifdef I2S_NUM_1
        } else if (port == I2S_NUM_1) {
            i2s_config->bck_io_num = pins.i2s1_bck;
            i2s_config->ws_io_num = pins.i2s1_ws;
            i2s_config->data_out_num = pins.i2s1_out;
            i2s_config->data_in_num = pins.i2s1_in;
            i2s_config->mck_io_num = pins.i2s1_mclk;
#endif
        } else {
            //memset(i2s_config, -1, sizeof(i2s_pin_config_t));
            KIT_LOGE("i2s port %d is not supported", port);
            return ESP_FAIL;
        }

        KIT_LOGI("i2s bck_io_num: %d", i2s_config->bck_io_num);
        KIT_LOGI("i2s ws_io_num: %d", i2s_config->ws_io_num);
        KIT_LOGI("i2s data_out_num: %d", i2s_config->data_out_num);
        KIT_LOGI("i2s data_in_num: %d", i2s_config->data_in_num);

        return ESP_OK;
    }


    esp_err_t get_spi_pins(
        spi_bus_config_t* spi_config, spi_device_interface_config_t* spi_device_interface_config)
    {
        AUDIO_NULL_CHECK(TAG, spi_config, return ESP_FAIL);
        AUDIO_NULL_CHECK(TAG, spi_device_interface_config, return ESP_FAIL);

        spi_config->mosi_io_num = -1;
        spi_config->miso_io_num = -1;
        spi_config->sclk_io_num = -1;
        spi_config->quadwp_io_num = -1;
        spi_config->quadhd_io_num = -1;

        spi_device_interface_config->spics_io_num = -1;

        KIT_LOGW("SPI interface is not supported");
        return ESP_OK;
    }

    // sdcard

    int8_t get_sdcard_intr_gpio(void) { return pins.sd_intr; }

    int8_t get_sdcard_open_file_num_max(void) { return pins.sd_max_open_files; }

    // input-output pins

    int8_t get_auxin_detect_gpio(void) { return pins.detect_auxin; }

    int8_t get_headphone_detect_gpio(void) { return pins.detect_headphone; }

    int8_t get_pa_enable_gpio(void) { return pins.enable_pa; }

    // button pins

    int8_t get_input_rec_id(void) { return pins.key1; }

    int8_t get_input_mode_id(void) { return pins.key2; }

    int8_t get_input_set_id(void) { return pins.key3; }

    int8_t get_input_play_id(void) { return pins.key4; }

    int8_t get_input_volup_id(void) { return pins.key5; }

    int8_t get_input_voldown_id(void) { return pins.key6; }
    // led pins

    int8_t get_adc_detect_gpio() { return pins.adc_detect; }
    int8_t get_es7243_mclk_gpio() { return pins.es7243_mclk; }
    int8_t get_es8311_mclk_src() { return pins.es8311_mclk_src; }
    int8_t get_reset_codec_gpio() { return pins.reset_codec; }
    int8_t get_reset_board_gpio() { return pins.reset_board; }
    int8_t get_blue_led_gpio() { return pins.led_blue; }
    int8_t get_green_led_gpio(void) { return pins.led_green; }

protected:
    audio_kit_pins pins;

};
