/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2021 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "AudioKitSettings.h"
#if AUDIOKIT_MUTEX_SUPPORT==1
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#else
#define NULL 0
#endif

#include "audio_hal/audio_mutex.h"
#include "audio_hal/audio_version.h"
#include "audio_hal/audiokit_logger.h"

#if AUDIOKIT_MUTEX_SUPPORT==1 && (ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(4, 3, 0))
// The xQueueSemaphoreTake is not available on FreeRTOS v8.2.0, it's compatible implementation.
BaseType_t __attribute__((weak)) xQueueSemaphoreTake( QueueHandle_t xQueue, TickType_t xTicksToWait )
{
    xSemaphoreTake(xQueue, xTicksToWait);
    return pdPASS;
}
#endif

void *mutex_create(void)
{
    void *handle = NULL;
#if AUDIOKIT_MUTEX_SUPPORT==1
    handle = xSemaphoreCreateMutex();
#endif
    return (void *) handle;
}

int mutex_destroy(void *mutex)
{
#if AUDIOKIT_MUTEX_SUPPORT==1
    vSemaphoreDelete((QueueHandle_t)mutex);
#endif
    return 0;
}

int mutex_lock(void *mutex)
{
#if AUDIOKIT_MUTEX_SUPPORT==1
    while (xSemaphoreTake((QueueHandle_t)mutex, portMAX_DELAY) != pdPASS);
#endif
    return 0;
}

int mutex_unlock(void *mutex)
{
    int ret = 0;
#if AUDIOKIT_MUTEX_SUPPORT==1
    ret = xSemaphoreGive((QueueHandle_t)mutex);
#endif
    return ret;
}
