#if defined(ARDUINO) && !defined(ESP32)
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
    digitalWrite(pin, mode);
    return ESP_OK;
}

/**
 * @brief NOP method
 * 
 * @param pGPIOConfig 
 * @return esp_err_t 
 */
esp_err_t gpio_config(const gpio_config_t *pGPIOConfig){
    int mode = OUTPUT;
    switch(pGPIOConfig->mode){
        case GPIO_MODE_INPUT:
            mode = pGPIOConfig->pull_up_en ? INPUT_PULLUP : INPUT;
            break;
        case GPIO_MODE_OUTPUT:
            mode = OUTPUT;
            break;

    }
    for (int pin=0;pin<32;pin++){
        if (pGPIOConfig->pin_bit_mask>pin & 1){
          pinMode(pin, mode);
          KIT_LOGI("pinMode: %d", pin, mode);
        }
    }
    return ESP_OK;
}

int gpio_get_level(gpio_num_t gpio_num){
    return digitalRead(gpio_num);
}



#endif