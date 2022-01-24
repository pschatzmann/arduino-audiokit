/**
 * @file audio_types.h
 * @author Phil Schatzmann
 * @brief Platform independent audio types
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#ifdef ESP32
#include "audio_hal/audio_types.h"
#else
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#endif