/**
 * @file audiokit_logger.h
 * @author your name (you@domain.com)
 * @brief Simple Logger we need to support both C and C++ 
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

// maximum size of log string
#define AUDIOKIT_LOGLENGTH 215

// Logging of method
#define LOG_METHOD __PRETTY_FUNCTION__

#ifdef __cplusplus
extern "C" {
#endif

// define supported log levels
enum AudioKitLogLevels {AudioKitDebug, AudioKitInfo, AudioKitWarning, AudioKitError};

// define default log level
extern int LOGLEVEL_AUDIOKIT;

void KIT_LOGD(const char* fmr,...);
void KIT_LOGI(const char* fmr,...);
void KIT_LOGW(const char* fmr,...);
void KIT_LOGE(const char* fmr,...);

#ifdef __cplusplus
}
#endif
