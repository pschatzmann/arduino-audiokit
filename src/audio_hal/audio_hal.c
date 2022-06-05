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

#include "audio_hal/audiokit_logger.h"
#include <string.h>
#include "audio_hal/audio_gpio.h"
#include "audio_hal/audiokit_logger.h"
#include "audio_hal/audio_hal.h"

#include "audio_hal/audio_mem.h"
#include "audio_hal/audio_mutex.h"

#define TAG_HAL "AUDIO_HAL"

#define AUDIO_HAL_CHECK_NULL(a, format, b, ...) \
    if ((a) == 0) { \
        KIT_LOGE( format, ##__VA_ARGS__); \
        return b;\
    }



audio_hal_handle_t audio_hal_init(audio_hal_codec_config_t *audio_hal_conf, audio_hal_func_t *audio_hal_func)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t ret = 0;
    audio_hal_handle_t audio_hal = (audio_hal_handle_t) audio_calloc(1, sizeof(audio_hal_func_t));
    AUDIO_MEM_CHECK(TAG_HAL, audio_hal, return NULL);
    memcpy(audio_hal, audio_hal_func, sizeof(audio_hal_func_t));
    audio_hal->audio_hal_lock = mutex_create();

#if AUDIOKIT_MUTEX_SUPPORT==1
    AUDIO_MEM_CHECK(TAG_HAL, audio_hal->audio_hal_lock, {
        KIT_LOGE("AUDIO_MEM_CHECK");
        audio_free(audio_hal);
        return NULL;
    });
#endif
    mutex_lock(audio_hal->audio_hal_lock);

    ret  = audio_hal->audio_codec_initialize(audio_hal_conf);
    KIT_LOGD("audio_codec_initialize -> %d", ret);
    if (ret == ESP_FAIL) {
        mutex_unlock(audio_hal->audio_hal_lock);
        if (audio_hal_func->handle) {
            return audio_hal_func->handle;
        } else {
            audio_free(audio_hal);
            KIT_LOGE( "codec init failed!");
            return NULL;
        }
    } else {
        KIT_LOGI("audio_codec_initialize-END-OK");
    }

    ret |= audio_hal->audio_codec_config_iface(audio_hal_conf->codec_mode, &audio_hal_conf->i2s_iface);
    ret |= audio_hal->audio_codec_set_volume(AUDIO_HAL_VOL_DEFAULT);
    audio_hal->handle = audio_hal;
    audio_hal_func->handle = audio_hal;

    mutex_unlock(audio_hal->audio_hal_lock);
    return audio_hal;
}

esp_err_t audio_hal_deinit(audio_hal_handle_t audio_hal)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t ret;
    AUDIO_HAL_CHECK_NULL(audio_hal, "audio_hal handle is null", -1);
    mutex_destroy(audio_hal->audio_hal_lock);
    ret = audio_hal->audio_codec_deinitialize();
    audio_hal->audio_hal_lock = NULL;
    audio_hal->handle = NULL;
    audio_free(audio_hal);
    audio_hal = NULL;
    return ret;
}

esp_err_t audio_hal_ctrl_codec(audio_hal_handle_t audio_hal, audio_hal_codec_mode_t mode, audio_hal_ctrl_t audio_hal_state)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t ret;
    AUDIO_HAL_CHECK_NULL(audio_hal, "audio_hal handle is null", -1);
    KIT_LOGI( "Codec mode is %d, Ctrl:%d", mode, audio_hal_state);
    mutex_lock(audio_hal->audio_hal_lock);
    ret = audio_hal->audio_codec_ctrl(mode, audio_hal_state);
    mutex_unlock(audio_hal->audio_hal_lock);
    return ret;
}

esp_err_t audio_hal_codec_iface_config(audio_hal_handle_t audio_hal, audio_hal_codec_mode_t mode, audio_hal_codec_i2s_iface_t *iface)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t ret = 0;
    AUDIO_HAL_CHECK_NULL(audio_hal, "audio_hal handle is null", -1);
    AUDIO_HAL_CHECK_NULL(iface, "Get volume para is null", -1);
    mutex_lock(audio_hal->audio_hal_lock);
    ret = audio_hal->audio_codec_config_iface(mode, iface);
    mutex_unlock(audio_hal->audio_hal_lock);
    return ret;
}

esp_err_t audio_hal_set_mute(audio_hal_handle_t audio_hal, bool mute)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t ret;
    AUDIO_HAL_CHECK_NULL(audio_hal, "audio_hal handle is null", -1);
    mutex_lock(audio_hal->audio_hal_lock);
    ret = audio_hal->audio_codec_set_mute(mute);
    mutex_unlock(audio_hal->audio_hal_lock);
    return ret;
}

esp_err_t audio_hal_set_volume(audio_hal_handle_t audio_hal, int volume)
{
    KIT_LOGD("audio_hal_set_volume: %d", volume);
    esp_err_t ret;
    AUDIO_HAL_CHECK_NULL(audio_hal, "audio_hal handle is null", -1);
    mutex_lock(audio_hal->audio_hal_lock);
    ret = audio_hal->audio_codec_set_volume(volume);
    mutex_unlock(audio_hal->audio_hal_lock);
    return ret;
}

esp_err_t audio_hal_get_volume(audio_hal_handle_t audio_hal, int *volume)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t ret;
    AUDIO_HAL_CHECK_NULL(audio_hal, "audio_hal handle is null", -1);
    AUDIO_HAL_CHECK_NULL(volume, "Get volume para is null", -1);
    mutex_lock(audio_hal->audio_hal_lock);
    ret = audio_hal->audio_codec_get_volume(volume);
    mutex_unlock(audio_hal->audio_hal_lock);
    return ret;
}

