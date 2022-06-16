/**
 * @file i2c_p_wire->cpp
 * @author Phil Schatzmann
 * @brief I2S Implementation using Arduino Wire Library
 * @date 2021-12-12
 *
 * @copyright Copyright (c) 2021
 */

#include "AudioKitSettings.h"
#if AUDIOKIT_USE_WIRE==1

#include <Wire.h>
#include <stdio.h>

#include "audio_hal/audio_error.h"
#include "audio_hal/audiokit_logger.h"
#include "audio_hal/i2c_bus.h"
// 0x20
#define PORT 0x20
#define END true

TwoWire *p_wire = &AUDIOKIT_WIRE;

i2c_bus_handle_t i2c_bus_create(i2c_port_t port, i2c_config_t* conf)
{
    KIT_LOGD(LOG_METHOD);

#if defined(ESP32)
    KIT_LOGI("i2c sda: %d", conf->sda_io_num);
    KIT_LOGI("i2c scl: %d", conf->scl_io_num);
    p_wire->setPins(conf->sda_io_num, conf->scl_io_num);
    KIT_LOGI("i2c clk_speed: %d", conf->master.clk_speed);
#elif defined(ARDUINO_ARCH_RP2040)
    p_wire->setSDA(conf->sda_io_num);
    bool setSCL(conf->scl_io_num);
#else
#warning "Pins in Wire Library ignored"
#endif

    p_wire->begin();
    p_wire->setClock(conf->master.clk_speed);
    
    return nullptr;
}

esp_err_t i2c_bus_cmd_begin(i2c_bus_handle_t bus, i2c_cmd_handle_t cmd, portBASE_TYPE ticks_to_wait)
{
    KIT_LOGD(LOG_METHOD);
    return ESP_OK;
}

// this method is used !
esp_err_t i2c_bus_write_bytes(i2c_bus_handle_t bus, int addr, uint8_t* reg, int reglen, uint8_t* data, int datalen)
{
    KIT_LOGD("i2c_bus_write_bytes: addr=%d reglen=%d datalen=%d - reg=%d value=%d", addr, reglen, datalen, reg[0], data[0]);

    assert(reglen == 1);
    assert(datalen == 1);

    int result = ESP_OK;
    p_wire->beginTransmission(addr >> 1);
    p_wire->write(reg[0]);
    p_wire->write(data[0]);
    int rc = p_wire->endTransmission(END);
    if (rc != 0) {
        KIT_LOGE("->p_wire->endTransmission: %d", rc);
        result = ESP_FAIL;
    }
    return result;
}

esp_err_t i2c_bus_write_data(i2c_bus_handle_t bus, int addr, uint8_t* data, int datalen)
{
    KIT_LOGD("i2c_bus_write_data: addr=%d len=%d", addr, datalen);
    assert(datalen == 1);

    int result = ESP_OK;
    p_wire->beginTransmission(addr >> 1);
    p_wire->write(data, datalen);
    int rc = p_wire->endTransmission(END);
    if (rc != 0) {
        KIT_LOGE("->p_wire->endTransmission: %d", rc);
        result = ESP_FAIL;
    }
    return result;
}

/// This method is used
esp_err_t i2c_bus_read_bytes(i2c_bus_handle_t bus, int addr, uint8_t* reg, int reglen, uint8_t* outdata, int datalen)
{
    KIT_LOGD("i2c_bus_read_bytes: addr=%d reglen=%d datalen=%d - reg=%d", addr, reglen, datalen, reg[0]);
    assert(reglen == 1);
    assert(datalen == 1);

    outdata[0] = 0;
    int result = ESP_OK;

    p_wire->beginTransmission(addr >> 1);
    p_wire->write(reg[0]);
    int rc = p_wire->endTransmission();
    if (rc!=0){
        KIT_LOGE("->p_wire->endTransmission: %d", rc);
    }

    uint8_t result_len = p_wire->requestFrom((uint16_t)(addr >> 1), (uint8_t)1, true);
    if (result_len > 0) {
        result_len = p_wire->readBytes(outdata, datalen);
    } else {
        KIT_LOGE("->p_wire->requestFrom %d->%d", datalen, result_len);
        result = ESP_FAIL;
    }
    return result;
}

esp_err_t i2c_bus_delete(i2c_bus_handle_t bus)
{
    KIT_LOGD(LOG_METHOD);
// All arduino implementations except old IDF versions
#if !defined(ESP32) || ESP_IDF_VERSION_MAJOR >= 4    
    p_wire->end();
#endif

    return ESP_OK;
}

#endif