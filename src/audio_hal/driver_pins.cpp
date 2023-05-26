#include "audio_hal/driver_pins.h"
#include "audio_hal/audiokit_board.h"

static board_driver *p_audiokit_current_driver=nullptr;

void set_driver_pins(board_driver &driver) { p_audiokit_current_driver = &driver; }

esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t* i2c_config){
    return p_audiokit_current_driver->get_i2c_pins(port, i2c_config);
}

int8_t get_es8311_mclk_src(void) {
    return p_audiokit_current_driver->get_es8311_mclk_src();
}

int8_t get_es7243_mclk_gpio(void){
    return p_audiokit_current_driver->get_es7243_mclk_gpio();
}

int8_t get_pa_enable_gpio(void){
    return p_audiokit_current_driver->get_pa_enable_gpio();
}