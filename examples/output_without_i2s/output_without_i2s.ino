/**
 * @file output_without_i2s.ino
 * @author Phil Schatzmann
 * @brief We just set up the AudioKit w/o I2S. The I2S is done with some other API
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AudioKitHAL.h"
#include "SineWaveGenerator.h"

AudioKit kit;
SineWaveGenerator wave;
const int BUFFER_SIZE = 1024;
uint8_t buffer[BUFFER_SIZE];
size_t bytesWritten;
i2s_pin_config_t pin_config;

void setup() {
  Serial.begin(115200);
  LOGLEVEL_AUDIOKIT = AudioKitInfo; 

  // Setup Codec chip only w/o I2S
  auto cfg = kit.defaultConfig(KitOutput);
  cfg.i2s_active = false;
  cfg.sample_rate = AUDIO_HAL_08K_SAMPLES;
  kit.begin(cfg);

  // setup I2S using esp32 API
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 8000,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 512,
    .use_apll = false
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  get_i2s_pins(I2S_NUM_0, &pin_config);  // get default i2s pins from audiokit 
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_start(I2S_NUM_0);

  // 1000 hz
  wave.setFrequency(1000);
  wave.setSampleRate(cfg.sampleRate());

}

void loop() {
  size_t l = wave.read(buffer, BUFFER_SIZE);
  // esp32 I2S API
  i2s_write(I2S_NUM_0, buffer, l, &bytesWritten, portMAX_DELAY); 
}