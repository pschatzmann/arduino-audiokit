/**
 * @file sdmmc_begin.ino
 * @author Phil Schatzmann
 * @brief  A simple test if we can start the SD via SDMMC of the AudioKit
 * @date 2022-5-12
 *
 * On the AI Thinker boards the recommended pin settings are  On, Off, On, Off, Off.
 * For further details see https://github.com/pschatzmann/arduino-audiokit/wiki/Problems-with-the-SD-Drive-or-some-conflicting-Buttons#sdmmc-library
 *
 * @copyright Copyright (c) 2021
 * 
 */

#include "AudioKitHAL.h"
#include "SD_MMC.h"

AudioKit kit;

void setup(){
    Serial.begin(115200);
    auto cfg = kit.defaultConfig();
    cfg.sd_active = false;
    kit.begin(cfg);
    if(!SD_MMC.begin("/sdcard", true)){    // 1-bit mode
        Serial.println("Card Mount Failed");
        return;
    } else {
        Serial.println("Card Mount Success");
    }
}

void loop() {

}
