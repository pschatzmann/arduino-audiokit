/**
 * @file sdbegin.ino
 * @author Phil Schatzmann
 * @brief  A simple test if we can start the SD of the AudioKit
 * @date 2021-12-10
 * 
 * On the AI Thinker boards the pin settings should be Off, On, On, Off, Off,
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AudioKitHAL.h"

#include "SD.h"

AudioKit kit;

void setup(){
    Serial.begin(115200);
    if(!SD.begin(kit.pinSpiCs(), AUDIOKIT_SD_SPI)){
        Serial.println("Card Mount Failed");
        return;
    } else {
        Serial.println("Card Mount Success");
    }
}

void loop() {

}