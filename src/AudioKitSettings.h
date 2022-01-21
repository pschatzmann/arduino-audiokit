/**
 * @file AudioKitSettings.h
 * @author Phil Schatzmann
 * @brief AUDIOKIT_BOARD selects a specic board:
 *  1) lyrat_v4_3
 *  2) lyrat_v4_2
 *  3) lyrat_mini_v1_1
 *  4) esp32_s2_kaluga_1_v1_2
 *  5) ai_thinker_v2_2
 *  6) generci_es8388
 * @version 0.1
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#ifndef AUDIOKIT_BOARD
#define AUDIOKIT_BOARD 5
#endif

// Default settings
#define AUDIOKIT_DEFAULT_INPUT          AUDIO_HAL_ADC_INPUT_LINE1
#define AUDIOKIT_DEFAULT_OUTPUT         AUDIO_HAL_DAC_OUTPUT_ALL
#define AUDIOKIT_DEFAULT_MASTER_SLAVE   AUDIO_HAL_MODE_SLAVE
#define AUDIOKIT_DEFAULT_RATE           AUDIO_HAL_44K_SAMPLES
#define AUDIOKIT_DEFAULT_BITSIZE        AUDIO_HAL_BIT_LENGTH_16BITS
#define AUDIOKIT_DEFAULT_I2S_FMT        AUDIO_HAL_I2S_NORMAL

// Activate or deactivate the following with 0 and 1
#ifndef AUDIOKIT_USE_WIRE
#define AUDIOKIT_USE_WIRE 1
#endif

#ifdef ESP32
#define AUDIOKIT_FREE_RTOS
#define AUDIOKIT_MUTEX_SUPPORT
#define AUDIOKIT_SETUP_SD
#else

#endif