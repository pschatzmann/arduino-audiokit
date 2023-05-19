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
#include <string.h>
#include "AudioKitSettings.h"
#include "es8388.h"
#include "audio_hal/board_pins_config.h"
#include "audio_hal/audiokit_logger.h"
#include "audio_hal/audio_gpio.h"

static i2c_bus_handle_t i2c_handle;
static int dac_power = 0x3c;

#define ES_ASSERT(a, format, b, ...) \
    if ((a) != 0) { \
        KIT_LOGE( format, ##__VA_ARGS__); \
        return b;\
    }

audio_hal_func_t AUDIO_CODEC_ES8388_DEFAULT_HANDLE = {
    .audio_codec_initialize = es8388_init,
    .audio_codec_deinitialize = es8388_deinit,
    .audio_codec_ctrl = es8388_ctrl_state,
    .audio_codec_config_iface = es8388_config_i2s,
    .audio_codec_set_mute = es8388_set_voice_mute,
    .audio_codec_set_volume = es8388_set_voice_volume,
    .audio_codec_get_volume = es8388_get_voice_volume,
    .audio_hal_lock = NULL,
    .handle = NULL,
};

static esp_err_t es_write_reg(uint8_t slave_addr, uint8_t reg_add, uint8_t data)
{
    return i2c_bus_write_bytes(i2c_handle, slave_addr, &reg_add, sizeof(reg_add), &data, sizeof(data));
}

static esp_err_t es_read_reg(uint8_t reg_add, uint8_t *p_data)
{
    return i2c_bus_read_bytes(i2c_handle, ES8388_ADDR, &reg_add, sizeof(reg_add), p_data, 1);
}

static int i2c_init()
{
    KIT_LOGD(LOG_METHOD);
    int res;
    i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_CLOCK_SPEED
    };
    res = get_i2c_pins(I2C_NUM_0, &es_i2c_cfg);
    ES_ASSERT(res, "getting i2c pins error", -1);
    i2c_handle = i2c_bus_create(I2C_NUM_0, &es_i2c_cfg);
    return res;
}

void es8388_read_all()
{
    KIT_LOGD(LOG_METHOD);
    for (int i = 0; i < 50; i++) {
        uint8_t reg = 0;
        es_read_reg(i, &reg);
        KIT_LOGI("%x: %x\n", i, reg);
    }
}

esp_err_t es8388_write_reg(uint8_t reg_add, uint8_t data)
{
    return es_write_reg(ES8388_ADDR, reg_add, data);
}

/**
 * @brief Configure ES8388 ADC and DAC volume. Basicly you can consider this as ADC and DAC gain
 *
 * @param mode:             set ADC or DAC or all
 * @param volume:           -96 ~ 0              for example Es8388SetAdcDacVolume(ES_MODULE_ADC, 30, 6); means set ADC volume -30.5db
 * @param dot:              whether include 0.5. for example Es8388SetAdcDacVolume(ES_MODULE_ADC, 30, 4); means set ADC volume -30db
 *
 * @return
 *     - (-1) Parameter error
 *     - (0)   Success
 */
static int es8388_set_adc_dac_volume(int mode, int volume, int dot)
{
    KIT_LOGD("es8388_set_adc_dac_volume: %d", volume);
    int res = 0;
    if ( volume < -96 || volume > 0 ) {
        KIT_LOGW("Warning: volume < -96! or > 0!\n");
        if (volume < -96)
            volume = -96;
        else
            volume = 0;
    }
    dot = (dot >= 5 ? 1 : 0);
    volume = (-volume << 1) + dot;
    if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
        res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL8, volume);
        res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL9, volume);  //ADC Right Volume=0db
    }
    if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL5, volume);
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL4, volume);
    }
    return res;
}


/**
 * @brief Power Management
 *
 * @param mod:      if ES_POWER_CHIP, the whole chip including ADC and DAC is enabled
 * @param enable:   false to disable true to enable
 *
 * @return
 *     - (-1)  Error
 *     - (0)   Success
 */
esp_err_t es8388_start(es_module_t mode)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    uint8_t prev_data = 0, data = 0;
    es_read_reg(ES8388_DACCONTROL21, &prev_data);
    if (mode == ES_MODULE_LINE) {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL16, 0x09); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL17, 0x50); // left DAC to left mixer enable  and  LIN signal to left mixer enable 0db  : bupass enable
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL20, 0x50); // right DAC to right mixer enable  and  LIN signal to right mixer enable 0db : bupass enable
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL21, 0xC0); //enable adc
    } else {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL21, 0x80);   //enable dac
    }
    es_read_reg(ES8388_DACCONTROL21, &data);
    if (prev_data != data) {
        res |= es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0xF0);   //start state machine
        // res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x16);
        // res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x50);
        res |= es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0x00);   //start state machine
    }
    if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC || mode == ES_MODULE_LINE) {
        res |= es_write_reg(ES8388_ADDR, ES8388_ADCPOWER, 0x00);   //power up adc and line in
        KIT_LOGD("es8388_start default is mode:%d", mode);
    }
    if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC || mode == ES_MODULE_LINE) {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, dac_power);   //power up dac and line out
        res |= es8388_set_voice_mute(false);
        KIT_LOGD("es8388_start default is mode:%d", mode);
    }

    return res;
}

/**
 * @brief Power Management
 *
 * @param mod:      if ES_POWER_CHIP, the whole chip including ADC and DAC is enabled
 * @param enable:   false to disable true to enable
 *
 * @return
 *     - (-1)  Error
 *     - (0)   Success
 */
esp_err_t es8388_stop(es_module_t mode)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    if (mode == ES_MODULE_LINE) {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL21, 0x80); //enable dac
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL17, 0x90); // only left DAC to left mixer enable 0db
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL20, 0x90); // only right DAC to right mixer enable 0db
        return res;
    }
    if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, 0x00);
        res |= es8388_set_voice_mute(true); 
        //res |= Es8388SetAdcDacVolume(ES_MODULE_DAC, -96, 5);      // 0db
        //res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, 0xC0);  //power down dac and line out
    }
    if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
        //res |= Es8388SetAdcDacVolume(ES_MODULE_ADC, -96, 5);      // 0db
        res |= es_write_reg(ES8388_ADDR, ES8388_ADCPOWER, 0xFF);  //power down adc and line in
    }
    if (mode == ES_MODULE_ADC_DAC) {
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL21, 0x9C);  //disable mclk
//        res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x00);
//        res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x58);
//        res |= es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0xF3);  //stop state machine
    }

    return res;
}


/**
 * @brief Config I2s clock in MSATER mode
 *
 * @param cfg.sclkDiv:      generate SCLK by dividing MCLK in MSATER mode
 * @param cfg.lclkDiv:      generate LCLK by dividing MCLK in MSATER mode
 *
 * @return
 *     - (-1)  Error
 *     - (0)   Success
 */
esp_err_t es8388_i2s_config_clock(es_i2s_clock_t cfg)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    res |= es_write_reg(ES8388_ADDR, ES8388_MASTERMODE, cfg.sclk_div);
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL5, cfg.lclk_div);  //ADCFsMode,singel SPEED,RATIO=256
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL2, cfg.lclk_div);  //ADCFsMode,singel SPEED,RATIO=256
    return res;
}

esp_err_t es8388_deinit(void)
{
    KIT_LOGD(LOG_METHOD);
    int res = 0;
    res = es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0xFF);  //reset and stop es8388
    i2c_bus_delete(i2c_handle);
#ifdef CONFIG_ESP_LYRAT_V4_3_BOARD
    headphone_detect_deinit();
#endif

    return res;
}

/**
 * @return
 *     - (-1)  Error
 *     - (0)   Success
 */
esp_err_t es8388_init(audio_hal_codec_config_t *cfg)
{
    KIT_LOGD(LOG_METHOD);

    int res = 0;
#ifdef CONFIG_ESP_LYRAT_V4_3_BOARD
    headphone_detect_init(get_headphone_detect_gpio());
#endif

    res = i2c_init(); // ESP32 in master mode

    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL3, 0x04);  // 0x04 mute/0x00 unmute&ramp;DAC unmute and  disabled digital volume control soft ramp
    /* Chip Control and Power Management */
    res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x50);
    res |= es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0x00); //normal all and power up all

    // Disable the internal DLL to improve 8K sample rate
    res |= es_write_reg(ES8388_ADDR, 0x35, 0xA0);
    res |= es_write_reg(ES8388_ADDR, 0x37, 0xD0);
    res |= es_write_reg(ES8388_ADDR, 0x39, 0xD0);

    res |= es_write_reg(ES8388_ADDR, ES8388_MASTERMODE, cfg->i2s_iface.mode); //CODEC IN I2S SLAVE MODE

    /* dac */
    res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, 0xC0);  //disable DAC and disable Lout/Rout/1/2
    res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x12);  //Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
//    res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0);  //LPVrefBuf=0,Pdn_ana=0
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL1, 0x18);//1a 0x18:16bit iis , 0x00:24
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL2, 0x02);  //DACFsMode,SINGLE SPEED; DACFsRatio,256
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL17, 0x90); // only left DAC to left mixer enable 0db
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL20, 0x90); // only right DAC to right mixer enable 0db
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL21, 0x80); //set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL23, 0x00);   //vroi=0
    res |= es8388_set_adc_dac_volume(ES_MODULE_DAC, 0, 0);          // 0db
    dac_power = 0;
    KIT_LOGI("dac_output %d",cfg->dac_output);
    if (AUDIO_HAL_DAC_OUTPUT_LINE2 == cfg->dac_output) {
        dac_power = DAC_OUTPUT_LOUT1 | DAC_OUTPUT_ROUT1;
    } else if (AUDIO_HAL_DAC_OUTPUT_LINE1 == cfg->dac_output) {
        dac_power = DAC_OUTPUT_LOUT2 | DAC_OUTPUT_ROUT2;
    } else {
        dac_power = DAC_OUTPUT_LOUT1 | DAC_OUTPUT_LOUT2 | DAC_OUTPUT_ROUT1 | DAC_OUTPUT_ROUT2;
    }
    res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, dac_power);  //0x3c Enable DAC and Enable Lout/Rout/1/2
    /* adc */
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCPOWER, 0xFF);
    //
    //res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL1, 0xbb); // MIC Left and Right channel PGA gain
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL1, ES8388_DEFAULT_MIC_GAIN); // MIC Left and Right channel PGA gain
    int tmp = 0;
    if (AUDIO_HAL_ADC_INPUT_LINE1 == cfg->adc_input) {
#ifdef WORKAROUND_MIC_LINEIN_MIXED //See https://www.pschatzmann.ch/home/2021/12/15/the-ai-thinker-audiokit-audio-input-bug/
		es8388_set_mic_gain(MIC_GAIN_MIN); // Mute mic as work around for HW bug of mixed microphone and LineIN
		tmp = ADC_INPUT_LINPUT2_RINPUT2;
#else
        tmp = ADC_INPUT_LINPUT1_RINPUT1;
#endif
    } else if (AUDIO_HAL_ADC_INPUT_LINE2 == cfg->adc_input) { 
        tmp = ADC_INPUT_LINPUT2_RINPUT2;
    } else {
        tmp = ADC_INPUT_DIFFERENCE;
    }
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL2, tmp);  //0x00 LINSEL & RINSEL, LIN1/RIN1 as ADC Input; DSSEL,use one DS Reg11; DSR, LINPUT1-RINPUT1
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL3, 0x02);
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL5, 0x02);  //ADCFsMode,singel SPEED,RATIO=256
    //ALC for Microphone
    res |= es8388_set_adc_dac_volume(ES_MODULE_ADC, 0, 0);      // 0db
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCPOWER, 0x09); //Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode
    //es8388_pa_power(cfg->dac_output!=AUDIO_HAL_DAC_OUTPUT_LINE2);
    // KIT_LOGI("init,out:%02x, in:%02x", cfg->dac_output, cfg->adc_input);
    return res;
}

/**
 * @brief Configure ES8388 I2S format
 *
 * @param mode:           set ADC or DAC or all
 * @param bitPerSample:   see Es8388I2sFmt
 *
 * @return
 *     - (-1) Error
 *     - (0)  Success
 */
esp_err_t es8388_config_fmt(es_module_t mode, es_i2s_fmt_t fmt)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    uint8_t reg = 0;
    if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
        res = es_read_reg(ES8388_ADCCONTROL4, &reg);
        reg = reg & 0xfc;
        res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL4, reg | fmt);
    }
    if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
        res = es_read_reg(ES8388_DACCONTROL1, &reg);
        reg = reg & 0xf9;
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL1, reg | (fmt << 1));
    }
    return res;
}

/**
 * @brief Sets the output volume
 * @param volume: 0 ~ 100
 *
 * @return
 *     - (-1)  Error
 *     - (0)   Success
 */
#if AI_THINKER_ES8388_VOLUME_HACK==1

esp_err_t es8388_set_voice_volume(int volume) {
  KIT_LOGD("es8388_set_voice_volume (HACK 1): %d", volume);
  esp_err_t res = ESP_OK;
  if (volume < 0) volume = 0;
  else if (volume > 100) volume = 100;
  volume /= 3;
  res = es_write_reg(ES8388_ADDR, ES8388_DACCONTROL4, 0);
  res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL5, 0);
  // LOUT1 RLOUT1 volume: dataheet says only 6 bits
  res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL24, volume);
  res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL25, volume);
  // DAC LDACVOL RDACVOL default 0 = 0DB; Default value 192 = – -96 dB
  res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL26, volume);
  res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL27, volume);
  return res;
}  

#elif AI_THINKER_ES8388_VOLUME_HACK==2

esp_err_t es8388_set_voice_volume(int volume) {
    KIT_LOGD("es8388_set_voice_volume (HACK 2): %d", volume);
    esp_err_t res = ESP_OK;
    if (volume < 0) volume = 0;
    else if (volume > 100) volume = 100;
    volume = (volume*63)/100; 
    // LOUT1 RLOUT1 volume: dataheet says only 6 bits
    res = es_write_reg(ES8388_ADDR, ES8388_DACCONTROL24, volume);
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL25, volume);
    // DAC LDACVOL RDACVOL default 0 = 0DB; Default value 192 = – -96 dB
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL26, 0);
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL27, 0);
    // 30-bit a coefficient for shelving filter
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL8, 192 >> 2);
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL9, 192 >> 2);
    return res;
}

#else

esp_err_t es8388_set_voice_volume(int volume) {
    KIT_LOGD("es8388_set_voice_volume: %d", volume);
    esp_err_t res = ESP_OK;
    if (volume < 0)
        volume = 0;
    else if (volume > 100)
        volume = 100;
    volume /= 3;
    // ROUT1VOL LOUT1VOL 0 -> -45dB; 33 -> – 4.5dB
    res = es_write_reg(ES8388_ADDR, ES8388_DACCONTROL24, volume);
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL25, volume);
    // DAC LDACVOL RDACVOL default 0 = 0DB; Default value 192 = – -96 dB
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL26, 0);
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL27, 0);
    return res;
}
#endif

/**
 * Provides the volume
 * @return
 *           volume
 */
esp_err_t es8388_get_voice_volume(int *volume)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    uint8_t reg = 0;
    res = es_read_reg(ES8388_DACCONTROL24, &reg);
    if (res == ESP_FAIL) {
        *volume = 0;
    } else {
        *volume = reg;
        *volume *= 3;
        if (*volume == 99)
            *volume = 100;
    }
    return res;
}

/**
 * @brief Configure ES8388 data sample bits
 *
 * @param mode:             set ADC or DAC or all
 * @param bitPerSample:   see BitsLength
 *
 * @return
 *     - (-1) Parameter error
 *     - (0)   Success
 */
esp_err_t es8388_set_bits_per_sample(es_module_t mode, es_bits_length_t bits_length)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    uint8_t reg = 0;
    int bits = (int)bits_length;

    if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
        res = es_read_reg(ES8388_ADCCONTROL4, &reg);
        reg = reg & 0xe3;
        res |=  es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL4, reg | (bits << 2));
    }
    if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
        res = es_read_reg(ES8388_DACCONTROL1, &reg);
        reg = reg & 0xc7;
        res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL1, reg | (bits << 3));
    }
    return res;
}

/**
 * @brief Configure ES8388 DAC mute or not. Basically you can use this function to mute the output or unmute
 *
 * @param enable: enable or disable
 *
 * @return
 *     - (-1) Parameter error
 *     - (0)   Success
 */
esp_err_t es8388_set_voice_mute(bool enable)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    uint8_t reg = 0;
    res = es_read_reg(ES8388_DACCONTROL3, &reg);
    reg = reg & 0xFB;
    res |= es_write_reg(ES8388_ADDR, ES8388_DACCONTROL3, reg | (((int)enable) << 2));
    return res;
}

esp_err_t es8388_get_voice_mute(void)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    uint8_t reg = 0;
    res = es_read_reg(ES8388_DACCONTROL3, &reg);
    if (res == ESP_OK) {
        reg = (reg & 0x04) >> 2;
    }
    return res == ESP_OK ? reg : res;
}

/**
 * @param gain: Config DAC Output
 *
 * @return
 *     - (-1) Parameter error
 *     - (0)   Success
 */
esp_err_t es8388_config_dac_output(es_dac_output_t output)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res;
    uint8_t reg = 0;
    res = es_read_reg(ES8388_DACPOWER, &reg);
    reg = reg & 0xc3;
    res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, reg | output);
    return res;
}

/**
 * @param gain: Config ADC input
 *
 * @return
 *     - (-1) Parameter error
 *     - (0)   Success
 */
esp_err_t es8388_config_adc_input(es_adc_input_t input)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res;
    uint8_t reg = 0;
    res = es_read_reg(ES8388_ADCCONTROL2, &reg);
    reg = reg & 0x0f;
    res |= es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL2, reg | input);
    return res;
}

/**
 * @param gain: see es_mic_gain_t
 *
 * @return
 *     - (-1) Parameter error
 *     - (0)   Success
 */
esp_err_t es8388_set_mic_gain(es_mic_gain_t gain)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res, gain_n;
    gain_n = (int)gain / 3;
    gain_n = (gain_n << 4) + gain_n;
    res = es_write_reg(ES8388_ADDR, ES8388_ADCCONTROL1, gain_n); //MIC PGA
    return res;
}

int es8388_ctrl_state(audio_hal_codec_mode_t mode, audio_hal_ctrl_t ctrl_state)
{
    KIT_LOGD(LOG_METHOD);
    int res = 0;
    int es_mode_t = 0;
    switch (mode) {
        case AUDIO_HAL_CODEC_MODE_ENCODE:
            es_mode_t  = ES_MODULE_ADC;
            break;
        case AUDIO_HAL_CODEC_MODE_LINE_IN:
            es_mode_t  = ES_MODULE_LINE;
            break;
        case AUDIO_HAL_CODEC_MODE_DECODE:
            es_mode_t  = ES_MODULE_DAC;
            break;
        case AUDIO_HAL_CODEC_MODE_BOTH:
            es_mode_t  = ES_MODULE_ADC_DAC;
            break;
        default:
            es_mode_t = ES_MODULE_DAC;
            KIT_LOGW("Codec mode not support, default is decode mode");
            break;
    }
    if (AUDIO_HAL_CTRL_STOP == ctrl_state) {
        res = es8388_stop(es_mode_t);
    } else {
        res = es8388_start(es_mode_t);
        KIT_LOGD("start default is decode mode:%d", es_mode_t);
    }
    return res;
}

esp_err_t es8388_config_i2s(audio_hal_codec_mode_t mode, audio_hal_codec_i2s_iface_t *iface)
{
    KIT_LOGD(LOG_METHOD);
    esp_err_t res = ESP_OK;
    int tmp = 0;
    res |= es8388_config_fmt(ES_MODULE_ADC_DAC, iface->fmt);
    if (iface->bits == AUDIO_HAL_BIT_LENGTH_16BITS) {
        tmp = BIT_LENGTH_16BITS;
    } else if (iface->bits == AUDIO_HAL_BIT_LENGTH_24BITS) {
        tmp = BIT_LENGTH_24BITS;
    } else {
        tmp = BIT_LENGTH_32BITS;
    }
    res |= es8388_set_bits_per_sample(ES_MODULE_ADC_DAC, tmp);
    return res;
}

void es8388_pa_power(bool enable)
{
    KIT_LOGD("es8388_pa_power: %s", enable ? "true":"false");
	pinMode(get_pa_enable_gpio(), OUTPUT);
    if (enable) {
        digitalWrite(get_pa_enable_gpio(), HIGH); 
    } else {
        digitalWrite(get_pa_enable_gpio(), LOW); 
    }
}
