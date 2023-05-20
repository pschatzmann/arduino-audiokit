#include "AudioKitSettings.h"
// implement Arduino functions with the help of the Espressif IDF
#if !defined(ARDUINO) && (defined(ESP32)||defined(AUDIOKIT_USE_IDF))
#include "audio_gpio.h"

// Reimplement Arduino API with ESP32 ADF
void digitalWrite(int gpio, int value){
    gpio_set_level((gpio_num_t)gpio, value);
}

// Reimplement Arduino API with ESP32 ADF
int digitalRead(int gpio){
    return gpio_get_level((gpio_num_t) gpio);
}

// Reimplement Arduino API with ESP32 ADF
void pinMode(int gpio, int mode){
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = BIT64(gpio);

    switch(mode){
        case INPUT:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        break;
        case OUTPUT:
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        break;
        case INPUT_PULLUP:
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;

        break;
    }
    gpio_config(&io_conf);

}


#endif