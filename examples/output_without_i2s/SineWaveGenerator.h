/**
 * @file SineWaveGenerator.h
 * @author Phil Schatzmann
 * @brief A simple sine wave generator 
 * @version 0.1
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <stdint.h>

class SineWaveGenerator {

    public:

        // the scale defines the max value which is generated
        SineWaveGenerator(float amplitude = 32767.0, float phase = 0.0){
            m_amplitude = amplitude;
            m_phase = phase;
        }

        /// Defines the frequency - after the processing has been started
        void setFrequency(uint16_t frequency) {
            this->m_frequency = frequency;
        }

        void setSampleRate(uint16_t sr){
            sample_rate = sr;
            this->m_deltaTime = 1.0 / sample_rate;
        }

        /// Provides a single sample
        int16_t readSample() {
            float angle = double_Pi * m_frequency * m_time + m_phase;
            int16_t result = m_amplitude * sin(angle);
            m_time += m_deltaTime;
            return result;
        }

        /// filles the data with 2 channels
        size_t read(uint8_t *buffer, size_t bytes){
            size_t result = 0;
            int16_t *ptr = (int16_t*)buffer;
            for (int j=0;j<bytes/4;j++){
                int16_t sample = readSample();
                *ptr++ = sample;
                *ptr++ = sample;
                result+=4;
            }
            return result;
        }

    protected:
        int sample_rate;
        float m_frequency = 0;
        float m_time = 0.0;
        float m_amplitude = 1.0;  
        float m_deltaTime = 0.0;
        float m_phase = 0.0;
        float double_Pi = PI * 2.0;

};
