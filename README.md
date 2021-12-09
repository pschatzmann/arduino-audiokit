# Arduino AudioKit HAL

There are different ESP32 AudioKit boards available that can be programmed with the Espressif ADF Framework.

<img src="https://pschatzmann.github.io/arduino-audio-tools/resources/audio-toolkit.png" alt="Audio Kit" />

The ADF Framework contains an abstractioin layer to support different CODEC chips. Unfortunately ADF can not be used in Arduino, but it would be quite useful to have this functionality also available.

I converted the __audio_boards__ and their related __drivers__ into an __Arduino Library__ and provide an easy to use C++ class which configures both the CODEC and I2S. With this project it should get quite easy to use these boards also in Arduino.

## Configuration

You can define your board and the default settings in the ```AudioKitSettings.h``` file. 

```
 * @brief AUDIOKIT_BOARD selects a specic board:
 * 1) lyrat_v4_3
 * 2) lyrat_v4_2
 * 3) lyrat_mini_v1_1
 * 4) lyratd_msc_v2_2
 * 5) lyratd_msc_v2_1
 * 6) ai_thinker_v2_2
 * 7) esp32_s2_kaluga_1_v1_2
 */

#define AUDIOKIT_BOARD 1

```
Here the lyrat v3.4 has been selected. Alternatively you can specify the board in a sketch with a #define before the #includes as shown in the next example.


## Example Sketch

Here is an example sketch that writes audio data to the audio kit board via I2S. By default I2S is set up as master, the CODEC as slave, the sample size is 16 bits and the sample rate is 44.1k/sec if you did not change the default settings. 

You can adjust these parameters by setting your requested values in the cfg below:

```
#define AUDIOKIT_BOARD 7

#include "AudioKit.h"
#include "SineWaveGenerator.h"

AudioKit kit;
SineWaveGenerator wave;
const int BUFFER_SIZE = 1024;
uint8_t buffer[BUFFER_SIZE];

void setup() {
  // open in write mode
  auto cfg = kit.defaultConfig(true);
  cfg.sample_rate = AUDIO_HAL_22K_SAMPLES;
  kit.begin(cfg);

  // 1000 hz
  wave.setFrequency(1000);
  wave.setSampleRate(cfg.sampleRate());
}

void loop() {
  size_t l = wave.read(buffer, BUFFER_SIZE);
  kit.write(buffer, l);
}

```

## Documentation

Here is the documentaion of the [AudioKit class](https://pschatzmann.github.io/arduino-audiokit-hal/html/class_audio_kit.html)

## Status

Early Development



