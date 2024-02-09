#pragma once
// flag to indicate that the config supports pins
#define AUDIOKIT_V1

// if no ADC is available set it to NULL
#ifndef AUDIO_DRIVER_ADC
#  define AUDIO_DRIVER_ADC NULL
#endif

// Declarations if we need to Support a SD drive
#if AUDIOKIT_SETUP_SD
#  include "SPI.h"
#  ifdef ESP32
#    include "audio_hal/audio_system.h"
#    include "audio_hal/audio_type_def.h"
#    include "audio_hal/audio_version.h"
#    if !defined(CONFIG_IDF_TARGET_ESP32S3) && !defined(CONFIG_IDF_TARGET_ESP32S2)                     \
     && !defined(CONFIG_IDF_TARGET_ESP32C3)
static SPIClass SPI_VSPI(VSPI);
#    endif
#  endif
#endif

// Support for old ESP32 IDF versions
#if AUDIOKIT_ESP32_I2S
#  if ESP_IDF_VERSION_MAJOR < 4 && !defined(I2S_COMM_FORMAT_STAND_I2S)
#    define I2S_COMM_FORMAT_STAND_I2S (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB)
#    define I2S_COMM_FORMAT_STAND_MSB (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB)
#    define I2S_COMM_FORMAT_STAND_PCM_LONG (I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_PCM_LONG)
#    define I2S_COMM_FORMAT_STAND_PCM_SHORT (I2S_COMM_FORMAT_PCM | I2S_COMM_FORMAT_PCM_SHORT)
typedef int eps32_i2s_audio_sample_rate_type;
#  else
typedef uint32_t eps32_i2s_audio_sample_rate_type;
#  endif
#endif

// Define LED_BUILTIN
#if defined(GREEN_LED_GPIO)
#  undef LED_BUILTIN
#  define LED_BUILTIN GREEN_LED_GPIO
#endif

// When we do not have Arduino we need to provide the following implementations
#ifndef ARDUINO
#  ifdef AUDIOKIT_FREE_RTOS
void yield() { taskYIELD(); }
#  else
void yield() { }
#  endif
#endif
