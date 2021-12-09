#pragma once

/**
 * @brief AUDIOKIT_BOARD selects a specic board:
 * 1) lyrat_v4_3
 * 2) lyrat_v4_2
 * 3) lyrat_mini_v1_1
 * 4) lyratd_msc_v2_2
 * 5) lyratd_msc_v2_1
 * 6) ai_thinker_v2_2
 * 7) esp32_s2_kaluga_1_v1_2
 */

#ifndef AUDIOKIT_BOARD
#define AUDIOKIT_BOARD 1
#endif

#define AUDIOKIT_DEFAULT_INPUT          AUDIO_HAL_ADC_INPUT_LINE1
#define AUDIOKIT_DEFAULT_OUTPUT         AUDIO_HAL_DAC_OUTPUT_ALL
#define AUDIOKIT_DEFAULT_MASTER_SLAVE   AUDIO_HAL_MODE_SLAVE
#define AUDIOKIT_DEFAULT_RATE           AUDIO_HAL_44K_SAMPLES
#define AUDIOKIT_DEFAULT_BITSIZE        AUDIO_HAL_BIT_LENGTH_16BITS
#define AUDIOKIT_DEFAULT_I2S_FMT        AUDIO_HAL_I2S_NORMAL




