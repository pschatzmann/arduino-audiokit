/**
 * @file audiokit_board_select.h
 * @author Phil Schatzmann
 * @brief Select bard.h based on AUDIOKIT_BOARD definition in configuration file
 * @date 2023-5-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "AudioKitSettings.h"

// In the new logic we juse use the board_def.h
#if AUDIOKIT_BOARD==1
#include "audio_board/lyrat_v4_3.h"
#elif AUDIOKIT_BOARD==2
#include "audio_board/lyrat_v4_2.h"
#elif AUDIOKIT_BOARD==3
#include "audio_board/lyrat_mini_v1_1.h"
#elif AUDIOKIT_BOARD==4
#include "audio_board/esp32_s2_kaluga_1_v1_2.h"
#elif AUDIOKIT_BOARD==5
#include "audio_board/ai_thinker_es8388_5.h"
#elif AUDIOKIT_BOARD==6
#include "audio_board/ai_thinker_ac101.h"
#elif AUDIOKIT_BOARD==7
#include "audio_board/ai_thinker_es8388_7.h"
#elif AUDIOKIT_BOARD==8
#include "audio_board/esp32_s3_box.h"
#elif AUDIOKIT_BOARD==9
#include "audio_board/esp32_s3_box_lite.h"
#elif AUDIOKIT_BOARD==10
#include "audio_board/generic_es8388.h"
#elif AUDIOKIT_BOARD==11
#include "audio_board/generic_es8311.h"
#elif AUDIOKIT_BOARD==12
#include "audio_board/generic.h"
#endif




