/**
 * @file AudioKitSettings.h
 * @author Phil Schatzmann
 * @brief AUDIOKIT_BOARD selects a specic board:
 *  1) lyrat_v4_3
 *  2) lyrat_v4_2
 *  3) lyrat_mini_v1_1
 *  4) esp32_s2_kaluga_1_v1_2
 *  5) ai_thinker (ES8388) 2957 3478 A149
 *  6) ai_thinker (AC101) 2762 2957
 *  7) ai_thinker (ES8388) 2957
 *  10) generci_es8388
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

// 1 = Using Arduino Wire Library; 0 = Use ESP32 I2C API
#ifndef AUDIOKIT_USE_WIRE
#define AUDIOKIT_USE_WIRE 1
// for AUDIOKIT_WIRE use Wire or Wire1
#define AUDIOKIT_WIRE Wire1
#endif

#ifdef ESP32

#ifndef AUDIOKIT_FREE_RTOS
#define AUDIOKIT_FREE_RTOS 1
#endif

#ifndef AUDIOKIT_MUTEX_SUPPORT
#define AUDIOKIT_MUTEX_SUPPORT 1
#endif

// This reserves a SPI port
#ifndef AUDIOKIT_SETUP_SD
#define AUDIOKIT_SETUP_SD 1
// for AUDIOKIT_SD_SPI you can select SPI or SPI_VSPI
#define AUDIOKIT_SD_SPI SPI
#endif

// to increase the max volume e.g. for ai_thinker (ES8388) 2957 or A202 -> set to 1 or 2
#ifndef AI_THINKER_ES8388_VOLUME_HACK
#define AI_THINKER_ES8388_VOLUME_HACK 0
#endif

#endif


