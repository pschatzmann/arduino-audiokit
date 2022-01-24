#include "audio_hal/audiokit_logger.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


int AUDIOKIT_LOG_LEVEL = AudioKitWarning;

void KIT_LOGD(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= AudioKitDebug) { // AUDIOKIT_LOG_LEVEL = Debug
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Debug:   ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+9, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }
}

void KIT_LOGI(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= AudioKitInfo) { // AUDIOKIT_LOG_LEVEL = Info, Debug
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Info:    ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+9, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }
}

void KIT_LOGW(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= AudioKitWarning) { // AUDIOKIT_LOG_LEVEL = Warning, Info, Debug
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Warning:  ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+9, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }
}

void KIT_LOGE(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= AudioKitError) { // for all levels
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Error:   ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+9, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }

}

