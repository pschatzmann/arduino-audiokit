#ifndef ESP32
#include "Arduino.h"
#include "audio_hal/audio_gpio.h"
#include "audio_hal/audiokit_logger.h"

/**
 * @brief gpio_set_level using Arduino API
 * 
 * @param pin 
 * @param mode 
 * @return esp_err_t 
 */

esp_err_t gpio_set_level(gpio_num_t pin, int mode) {
    KIT_LOGI("gpio_set_level: %d - %d", pin, mode);
    pinMode(pin, mode);
    return ESP_OK;
}

/**
 * @brief NOP method
 * 
 * @param pGPIOConfig 
 * @return esp_err_t 
 */
esp_err_t gpio_config(const gpio_config_t *pGPIOConfig){
    return ESP_OK;
}


#endif