#include "audiokit_logger.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

void LOGD(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= Debug) {
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Debug:  ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+7, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }
}

void LOGI(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= Info) {
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Info:  ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+7, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }
}

void LOGW(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= Warning) {
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Warning:  ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+7, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }
}

void LOGE(const char* fmr, ...) {
  if (AUDIOKIT_LOG_LEVEL <= Error) {
    char log_buffer[AUDIOKIT_LOGLENGTH+1];
    strcpy(log_buffer, "Error:  ");
    va_list arg;
    va_start(arg, fmr);
    vsprintf(log_buffer+7, fmr, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf(log_buffer);
  }

}