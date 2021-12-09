#include "periph_sdcard.h"

#include "SPI.h"
#include "board_pins_config.h"
/**
 * @brief We just initialize SPI so that we can use any SD library
 *
 * @param sdcard_config
 * @return esp_periph_handle_t
 */
esp_periph_handle_t periph_sdcard_init(periph_sdcard_cfg_t* sdcard_config) {
  spi_bus_config_t spi_cfg;
  get_spi_pins(&spi_cfg, NULL);

  SPI.begin(spi_cfg.sclk_io_num, spi_cfg.miso_io_num, spi_cfg.mosi_io_num,
            sdcard_config->card_detect_pin);
  return NULL;
}

