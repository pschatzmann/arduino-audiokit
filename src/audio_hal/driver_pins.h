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
#include "driver/i2s.h"
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
