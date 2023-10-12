/**
 * @file driver_pins.h
 * @author Phil Schatzmann
 * @brief The drivers call come c methods to determine some critical pins.
 * Here we make these methods available.
 * @date 2023-5-25
 * 
 * @copyright Copyright (c) 2021
*/
#pragma once

#include "audio_hal/audio_gpio.h"
#ifdef ESP32
#include "hal/spi_types.h"
#include "driver/spi_slave.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
#include "driver/i2s.h"
#else
#define I2S_NUM_0 0
#ifndef I2S_PIN_NO_CHANGE
#define I2S_PIN_NO_CHANGE -1
#endif
typedef struct {
    int mck_io_num;
    int bck_io_num;     /*!< BCK in out pin*/
    int ws_io_num;      /*!< WS in out pin*/
    int data_out_num;   /*!< DATA out pin*/
    int data_in_num;    /*!< DATA in pin*/
} i2s_pin_config_t;

#endif

#endif

#ifdef __cplusplus
extern "C" {
class board_driver;

void set_driver_pins(board_driver &driver);

#endif


esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t* i2c_config);

int8_t get_es8311_mclk_src(void);
int8_t get_es7243_mclk_gpio(void);
int8_t get_pa_enable_gpio(void);

#ifdef __cplusplus
}
#endif
