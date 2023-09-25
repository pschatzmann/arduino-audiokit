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
    
    // setup SPI with the coorrect pins: This is not necessary if you call kit.begin() with the SD active.
    SPI.begin(PIN_AUDIO_KIT_SD_CARD_CLK, PIN_AUDIO_KIT_SD_CARD_MISO, PIN_AUDIO_KIT_SD_CARD_MOSI, PIN_AUDIO_KIT_SD_CARD_CS);

    // start SD
    if(!SD.begin(PIN_AUDIO_KIT_SD_CARD_CS, AUDIOKIT_SD_SPI)){
        Serial.println("Card Mount Failed");
        return;
    } else {
        Serial.println("Card Mount Success");
    }
}

void loop() {

}