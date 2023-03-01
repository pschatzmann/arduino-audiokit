/**
 * @file input-and-output.ino
 * @author Phil Schatzmann
 * @brief Input of audio data from the AudioKit microphones and output to speaker and earphones
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AudioKitHAL.h"

AudioKit kit;
const int BUFFER_SIZE = 1024;
uint8_t buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  // open in read mode
  auto cfg = kit.defaultConfig(AudioInputOutput);
  cfg.adc_input = AUDIO_HAL_ADC_INPUT_LINE1; // microphone
  cfg.sample_rate = AUDIO_HAL_16K_SAMPLES;
  kit.begin(cfg);
  kit.setVolume(20);
}

void loop() {
  size_t len = kit.read(buffer, BUFFER_SIZE);
  kit.write(buffer, len);
}