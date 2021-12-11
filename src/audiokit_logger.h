#pragma once
/**
 * @brief Simple Logger we need to support both C and C++ 
 */

// maximum size of log string
#define AUDIOKIT_LOGLENGTH 215

// Logging of method
#define LOG_METHOD __PRETTY_FUNCTION__

#ifdef __cplusplus
extern "C" {
#endif

// define supported log levels
enum AudioKitLogLevels {Debug, Info, Warning, Error};

// define default log level
extern int AUDIOKIT_LOG_LEVEL;

void LOGD(const char* fmr,...);
void LOGI(const char* fmr,...);
void LOGW(const char* fmr,...);
void LOGE(const char* fmr,...);

#ifdef __cplusplus
}
#endif
