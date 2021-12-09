#pragma once

#include "AudioKitSettings.h"

// include boards dependent on configuration
#if AUDIOKIT_BOARD==1
#include "audio_board/lyrat_v4_3/board.h"
#elif AUDIOKIT_BOARD==2
#include "audio_board/lyrat_v4_2/board.h"
#elif AUDIOKIT_BOARD==3
#include "audio_board/lyrat_mini_v1_1/board.h"
#elif AUDIOKIT_BOARD==4
#include "audio_board/lyratd_msc_v2_2/board.h"
#elif AUDIOKIT_BOARD==5
#include "audio_board/lyratd_msc_v2_1/board.h"
#elif AUDIOKIT_BOARD==6
#include "audio_board/ai_thinker_v2_2/board.h"
#elif AUDIOKIT_BOARD==7
#include "audio_board/esp32_s2_kaluga_1_v1_2/board.h"
#endif
