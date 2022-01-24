/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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
#include <stdlib.h>
#include "string.h"
#include "audio_hal/audio_system.h"
#include "audio_hal/audiokit_logger.h"
#include "audio_hal/audio_mem.h"

#if defined(ESP32)
#include "esp_heap_caps.h"
#include "esp_efuse.h"

void *audio_malloc(size_t size)
{
    KIT_LOGD(LOG_METHOD);
    void *data =  NULL;
    data = heap_caps_malloc(size, MALLOC_CAP_8BIT);
#ifdef ENABLE_AUDIO_MEM_TRACE
    KIT_LOGI( "malloc:%p, size:%d, called:0x%08x", data, size, (intptr_t)__builtin_return_address(0) - 2);
#endif
    return data;
}


void *audio_calloc(size_t nmemb, size_t size)
{
    KIT_LOGD("audio_calloc: %d * %d", nmemb, size);
    void *data =  NULL;
    data = heap_caps_malloc(nmemb * size,  MALLOC_CAP_8BIT);
    if (data) {
        memset(data, 0, nmemb * size);
    }
#ifdef ENABLE_AUDIO_MEM_TRACE
    KIT_LOGI( "calloc:%p, size:%d, called:0x%08x", data, size, (intptr_t)__builtin_return_address(0) - 2);
#endif
    return data;
}

#else

void *audio_malloc(size_t size)
{
    KIT_LOGD(LOG_METHOD);
    void *data =  NULL;
    data = malloc(size);
#ifdef ENABLE_AUDIO_MEM_TRACE
    KIT_LOGI( "malloc:%p, size:%d, called:0x%08x", data, size, (intptr_t)__builtin_return_address(0) - 2);
#endif
    return data;
}

void *audio_calloc(size_t nmemb, size_t size)
{
    KIT_LOGD(LOG_METHOD);
    void *data =  NULL;
    data = calloc(nmemb, size);
#ifdef ENABLE_AUDIO_MEM_TRACE
    KIT_LOGI( "calloc:%p, size:%d, called:0x%08x", data, size, (intptr_t)__builtin_return_address(0) - 2);
#endif
    return data;
}

#endif


void audio_free(void *ptr)
{
    KIT_LOGD(LOG_METHOD);
    free(ptr);
#ifdef ENABLE_AUDIO_MEM_TRACE
    KIT_LOGI("free:%p, called:0x%08x", ptr, (intptr_t)__builtin_return_address(0) - 2);
#endif
}




