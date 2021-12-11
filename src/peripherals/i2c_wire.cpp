#include "AudioKitSettings.h"

#ifdef AUDIOKIT_USE_WIRE

#include <Wire.h>
#include <stdio.h>
#include "i2c_bus.h"
#include "audiokit_logger.h"
#include "audio_error.h"


i2c_bus_handle_t i2c_bus_create(i2c_port_t port, i2c_config_t *conf){
    // Wire.begin(I2C_SDA, I2C_SCL);
    if (!Wire.begin(conf->sda_io_num,conf->scl_io_num)){
        LOGE("->Wire.begin");
    }
    return nullptr;
}

esp_err_t i2c_bus_cmd_begin(i2c_bus_handle_t bus, i2c_cmd_handle_t cmd, portBASE_TYPE ticks_to_wait){
    LOGD(LOG_METHOD);
    return ESP_OK;
}

// 0x20
#define PORT 0x20

// this method is used !
esp_err_t i2c_bus_write_bytes(i2c_bus_handle_t bus, int addr, uint8_t *reg, int reglen, uint8_t *data, int datalen){
    LOGD(LOG_METHOD);
    assert(addr == PORT);
    LOGD("->i2c addr: %d", addr);
    int result = ESP_OK;
    Wire.beginTransmission(addr);
    int len = Wire.write(reg, reglen);
    if (len!=reglen){
        LOGE("->Wire.write addr");
        result = ESP_FAIL;
    }
    len = Wire.write(data, datalen);
    if (len!=reglen){
        LOGE("->Wire.write data");
        result = ESP_FAIL;
    }
    int rc = Wire.endTransmission();
    if (rc!=0){
        LOGE("->Wire.endTransmission: %d",rc);
        result = ESP_FAIL;
    }
    return result;
}

esp_err_t i2c_bus_write_data(i2c_bus_handle_t bus, int addr, uint8_t *data, int datalen){
    LOGD(LOG_METHOD);
    assert(addr == PORT);
    int result = ESP_OK;
    Wire.beginTransmission(addr);
    int len = Wire.write(data,datalen);
    if (len!=datalen){
        LOGE("->Wire.write data");
        result = ESP_FAIL;
    }
    int rc = Wire.endTransmission();
    if (rc!=0){
        LOGE("->Wire.endTransmission: %d", rc);
        result = ESP_FAIL;
    }
    return result;
}

/// This method is used
esp_err_t i2c_bus_read_bytes(i2c_bus_handle_t bus, int addr, uint8_t *reg, int reglen, uint8_t *outdata, int datalen){
    LOGD(LOG_METHOD);
    assert(addr == PORT);
    int result = i2c_bus_write_bytes(bus, addr, reg, reglen, outdata, datalen);

    uint8_t result_len = Wire.requestFrom((uint16_t)addr, (uint8_t)datalen,(bool) true);
    LOGD("->Wire.requestFrom %d->%d", datalen, result_len);
    // wait for result
    while(Wire.available()<datalen){
         delay(10);
    }
    LOGD("->Wire.available %d->%d", datalen, Wire.available());
    for (int j=0;j<datalen;j++){
        outdata[j]=Wire.read();
    }
    return result;
}

esp_err_t i2c_bus_delete(i2c_bus_handle_t bus){
    LOGD(LOG_METHOD);
#ifndef ESP32
    Wire.end();
#endif
    return ESP_OK;
}



#endif