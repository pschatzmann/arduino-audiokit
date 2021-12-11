#include "AudioKitSettings.h"

#ifdef AUDIOKIT_USE_WIRE

#include <Wire.h>
#include <stdio.h>
#include "i2c_bus.h"
#include "audiokit_logger.h"

extern "C" {

i2c_bus_handle_t i2c_bus_create(i2c_port_t port, i2c_config_t *conf){
    LOGD(LOG_METHOD);
    Wire.begin();
    return nullptr;
}

esp_err_t i2c_bus_cmd_begin(i2c_bus_handle_t bus, i2c_cmd_handle_t cmd, portBASE_TYPE ticks_to_wait){
    LOGD(LOG_METHOD);
    return ESP_OK;
}

esp_err_t i2c_bus_write_bytes(i2c_bus_handle_t bus, int addr, uint8_t *reg, int regLen, uint8_t *data, int datalen){
    LOGD(LOG_METHOD);
    Wire.beginTransmission(addr);
    Wire.write(reg,regLen);
    Wire.write(data,datalen);
    Wire.endTransmission();
    return ESP_OK;
}

esp_err_t i2c_bus_write_data(i2c_bus_handle_t bus, int addr, uint8_t *data, int datalen){
    LOGD(LOG_METHOD);
    Wire.beginTransmission(addr);
    Wire.write(data,datalen);
    Wire.endTransmission();
    return ESP_OK;
}

esp_err_t i2c_bus_read_bytes(i2c_bus_handle_t bus, int addr, uint8_t *reg, int reglen, uint8_t *outdata, int datalen){
    LOGD(LOG_METHOD);
    Wire.beginTransmission(addr);
    Wire.write(reg, reglen);
    Wire.endTransmission(); 
    Wire.requestFrom(addr, datalen); 
    for (int j=0;j<datalen;j++){
        outdata[j]=Wire.read();
    }
    return ESP_OK;
}

esp_err_t i2c_bus_delete(i2c_bus_handle_t bus){
    LOGD(LOG_METHOD);
#ifndef ESP32
    Wire.end();
#endif
    return ESP_OK;
}

}

#endif