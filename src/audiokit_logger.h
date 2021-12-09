#pragma once

const char* AUDIOKIT = "AUDIOKIT";


#define LOGD(...)  ESP_LOGD(AUDIOKIT,__VA_ARGS__)
#define LOGI(...)  ESP_LOGI(AUDIOKIT,__VA_ARGS__)
#define LOGW(...)  ESP_LOGW(AUDIOKIT,__VA_ARGS__)
#define LOGE(...)  ESP_LOGE(AUDIOKIT,__VA_ARGS__)