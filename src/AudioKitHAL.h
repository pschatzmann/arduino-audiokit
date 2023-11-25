/**
 * @file AudioKit.h
 * @author Phil Schatzmann
 * @brief Arduino API for AudioKit
 * @version 0.1
 * @date 2021-12-12
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once
#include "AudioKitSettings.h"

// include drivers
#include "audio_driver/es7148/es7148.h"
#include "audio_driver/es7210/es7210.h"
#include "audio_driver/es7243/es7243.h"
#include "audio_driver/es8311/es8311.h"
#include "audio_driver/es8374/es8374.h"
#include "audio_driver/es8388/es8388.h"
#include "audio_driver/tas5805m/tas5805m.h"
#include "audio_hal/audio_gpio.h"
#include "audio_hal/audiokit_board.h"
#include "audio_hal/audiokit_board_select.h"
#include "audio_hal/audiokit_logger.h"
#include "audio_hal/driver_pins.h"
#include "audio_hal/common_defines.h"

namespace audiokit {

class AudioKit;
static AudioKit* selfAudioKit = nullptr;

/**
 * @brief Configuation for AudioKit
 *
 */
struct AudioKitConfig {
    /// @brief defines all relevant pins
    audio_kit_pins pins;
    /// @brief driver which selects the codec implementation: AUDIO_CODEC_ES8388_DEFAULT_HANDLE, AUDIO_CODEC_AC101_CODEC_HANDLE, AUDIO_CODEC_ES8311_DEFAULT_HANDLE, AUDIO_CODEC_ES7243_DEFAULT_HANDLE
    audio_hal_func_t driver = AUDIO_DRIVER;

    int i2s_num = pins.i2s_num;
    bool sd_active = true;
    bool auto_clear = true;
    bool use_apll = true;
    bool i2s_active = true;
    int buffer_count = 6;
    int buffer_size = 512;

    audio_hal_adc_input_t adc_input
        = AUDIOKIT_DEFAULT_INPUT; /*!<  set adc channel with audio_hal_adc_input_t*/
    audio_hal_dac_output_t dac_output = AUDIOKIT_DEFAULT_OUTPUT; /*!< set dac channel */
    audio_hal_codec_mode_t codec_mode; /*!< select codec mode: adc, dac or both */
    audio_hal_iface_mode_t master_slave_mode
        = AUDIOKIT_DEFAULT_MASTER_SLAVE; /*!< audio codec chip mode */
    audio_hal_iface_format_t fmt = AUDIOKIT_DEFAULT_I2S_FMT; /*!< I2S interface format */
    audio_hal_iface_samples_t sample_rate
        = AUDIOKIT_DEFAULT_RATE; /*!< I2S interface samples per second */
    audio_hal_iface_bits_t bits_per_sample
        = AUDIOKIT_DEFAULT_BITSIZE; /*!< i2s interface number of bits per sample */

    /// Returns true if the CODEC is the master
    bool isMaster() { return master_slave_mode == AUDIO_HAL_MODE_MASTER; }

    /// provides the bits per sample
    int bitsPerSample()
    {
        switch (bits_per_sample) {
        case AUDIO_HAL_BIT_LENGTH_16BITS:
            return 16;
        case AUDIO_HAL_BIT_LENGTH_24BITS:
            return 24;
        case AUDIO_HAL_BIT_LENGTH_32BITS:
            return 32;
        }
        KIT_LOGE("bits_per_sample not supported: %d", bits_per_sample);
        return 0;
    }

    /// Provides the sample rate in samples per second
    uint32_t sampleRate()
    {
        switch (sample_rate) {
        case AUDIO_HAL_08K_SAMPLES: /*!< set to  8k samples per second */
            return 8000;
        case AUDIO_HAL_11K_SAMPLES: /*!< set to 11.025k samples per second */
            return 11025;
        case AUDIO_HAL_16K_SAMPLES: /*!< set to 16k samples in per second */
            return 16000;
        case AUDIO_HAL_22K_SAMPLES: /*!< set to 22.050k samples per second */
            return 22050;
        case AUDIO_HAL_24K_SAMPLES: /*!< set to 24k samples in per second */
            return 24000;
        case AUDIO_HAL_32K_SAMPLES: /*!< set to 32k samples in per second */
            return 32000;
        case AUDIO_HAL_44K_SAMPLES: /*!< set to 44.1k samples per second */
            return 44100;
        case AUDIO_HAL_48K_SAMPLES: /*!< set to 48k samples per second */
            return 48000;
        }
        KIT_LOGE("sample rate not supported: %d", sample_rate);
        return 0;
    }

#if AUDIOKIT_ESP32_I2S && defined(ESP32)
    /// Provides the ESP32 i2s_config_t to configure I2S
    i2s_config_t i2sConfig()
    {
        // use just the oposite of the CODEC setting
        const i2s_config_t i2s_config = {
            .mode = i2sMode(),
            .sample_rate = sampleRate(),
            .bits_per_sample = (i2s_bits_per_sample_t)bitsPerSample(),
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = (i2s_comm_format_t)i2sFormat(),
            .intr_alloc_flags = 0, // default interrupt priority
            .dma_buf_count = buffer_count,
            .dma_buf_len = buffer_size,
            .use_apll = use_apll,
            .tx_desc_auto_clear = auto_clear,
        };
        return i2s_config;
    }

    /// Provides the ESP32 i2s_pin_config_t to configure the pins for I2S
    i2s_pin_config_t i2sPins()
    {
        i2s_pin_config_t result;
        board_driver board;
        board.setup(pins);
        board.get_i2s_pins(i2s_num, &result);
        return result;
    }

protected:
    i2s_comm_format_t i2sFormat()
    {
        i2s_comm_format_t its_com_fmt = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_I2S;
        if (fmt == AUDIO_HAL_I2S_LEFT) {
            its_com_fmt = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB;
        } else if (fmt == AUDIO_HAL_I2S_RIGHT) {
            its_com_fmt = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB;
        } else if (fmt == AUDIO_HAL_I2S_DSP) {
            its_com_fmt = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_PCM_SHORT;
        }
        return its_com_fmt;
    }

    i2s_mode_t i2sMode()
    {
        int mode = isMaster() ? I2S_MODE_SLAVE : I2S_MODE_MASTER;
        // using ESP32 dac/adc
        if (fmt == AUDIO_HAL_I2S_DSP) {
#if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)                       \
    || defined(CONFIG_IDF_TARGET_ESP32C3)
            KIT_LOGE("AUDIO_HAL_I2S_DSP not supported");
#else
            if (codec_mode == AUDIO_HAL_CODEC_MODE_DECODE) {
                mode = mode | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN;
            } else if (codec_mode == AUDIO_HAL_CODEC_MODE_ENCODE) {
                mode = mode | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN;
            } else if (codec_mode == AUDIO_HAL_CODEC_MODE_BOTH) {
                mode = mode | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_ADC_BUILT_IN
                    | I2S_MODE_DAC_BUILT_IN;
            }
#endif
        } else {
            // I2S
            if (codec_mode == AUDIO_HAL_CODEC_MODE_DECODE) {
                mode = mode | I2S_MODE_TX;
            } else if (codec_mode == AUDIO_HAL_CODEC_MODE_ENCODE) {
                mode = mode | I2S_MODE_RX;
            } else if (codec_mode == AUDIO_HAL_CODEC_MODE_BOTH) {
                mode = mode | I2S_MODE_RX | I2S_MODE_TX;
            }
        }
        return (i2s_mode_t)mode;
    }

#endif
};

/**
 * @brief Do we read or write audio data - or both
 *
 */
enum AudioKitInOut { KitOutput, KitInput, KitInputOutput };

/**
 * @brief AudioKit API using the audio_hal
 *
 */

class AudioKit {

public:
    AudioKit()
    {
        // setup SPI for SD drives
        selfAudioKit = this;
    }

    /// Provides the default configuration for input or output
    AudioKitConfig defaultConfig(AudioKitInOut inout = KitInputOutput)
    {
        AudioKitConfig result;
        switch (inout) {
        case KitOutput:
            result.codec_mode = AUDIO_HAL_CODEC_MODE_DECODE; // dac
            break;
        case KitInput:
            result.codec_mode = AUDIO_HAL_CODEC_MODE_ENCODE; // adc
            break;
        default:
            result.codec_mode = AUDIO_HAL_CODEC_MODE_BOTH;
            break;
        }
        return result;
    }

    /// Starts the codec
    bool begin(AudioKitConfig config)
    {
        KIT_LOGI(LOG_METHOD);
        cfg = config;

        KIT_LOGI("Selected board: %d", AUDIOKIT_BOARD);
        log_i2c();

        // setup driver pins
        driver.setup(cfg.pins);
        setupDriverPins();

        // if already active we stop first
        if (is_active) {
            end();
        }

#if AUDIOKIT_SETUP_SD
        // release SPI for SD card if it is not necessary
        if (AUDIOKIT_SETUP_SD && !cfg.sd_active) {
            p_spi->end();
        }
        setupSPI();
#endif

        // setup headphone if necessary
        setupHeadphoneDetection();

        audio_hal_conf.adc_input = cfg.adc_input;
        audio_hal_conf.dac_output = cfg.dac_output;
        audio_hal_conf.codec_mode = cfg.codec_mode;
        audio_hal_conf.i2s_iface.mode = cfg.master_slave_mode;
        audio_hal_conf.i2s_iface.fmt = cfg.fmt;
        audio_hal_conf.i2s_iface.samples = cfg.sample_rate;
        audio_hal_conf.i2s_iface.bits = cfg.bits_per_sample;

        // init HAL
        if (hal_handle == 0) {
            hal_handle = audio_hal_init(&audio_hal_conf, &cfg.driver);
            if (hal_handle == 0) {
                KIT_LOGE("audio_hal_init");
                return false;
            }
        }

        if (cfg.i2s_active) {
            setupI2S();
        }

        // call start
        if (!setActive(true)) {
            KIT_LOGE("setActive");
            return false;
        }

        // activate speaker when output not earphones only
        setSpeakerActive(cfg.dac_output != AUDIO_HAL_DAC_OUTPUT_LINE1);

        is_active = true;
        return true;
    }

    /// Stops the CODEC
    bool end()
    {
        KIT_LOGI(LOG_METHOD);

#if AUDIOKIT_ESP32_I2S && defined(ESP32)
        if (cfg.i2s_active) {
            // uninstall i2s driver
            i2s_driver_uninstall((i2s_port_t)cfg.i2s_num);
        }
#endif
        // stop codec driver
        audio_hal_ctrl_codec(hal_handle, cfg.codec_mode, AUDIO_HAL_CTRL_STOP);
        // deinit
        audio_hal_deinit(hal_handle);
        // this will save power
        setSpeakerActive(false);

        is_active = false;
        hal_handle = 0;
        return true;
    }

    /// Provides the actual configuration
    AudioKitConfig& config() { return cfg; }

    /// Sets the codec active / inactive
    bool setActive(bool active)
    {
        return audio_hal_ctrl_codec(
                   hal_handle, cfg.codec_mode, active ? AUDIO_HAL_CTRL_START : AUDIO_HAL_CTRL_STOP)
            == ESP_OK;
    }

    /// Mutes the output
    bool setMute(bool mute) { return audio_hal_set_mute(hal_handle, mute) == ESP_OK; }

    /// Defines the Volume (in %) if volume is 0, mute is enabled,range is 0-100.
    bool setVolume(int vol)
    {
        return (vol > 0) ? audio_hal_set_volume(hal_handle, vol) == ESP_OK : false;
    }

    /// Determines the volume %
    int volume()
    {
        int volume;
        if (audio_hal_get_volume(hal_handle, &volume) != ESP_OK) {
            volume = -1;
        }
        return volume;
    }

#if AUDIOKIT_ESP32_I2S && defined(ESP32)

    /// Writes the audio data via i2s to the DAC
    size_t write(const void* src, size_t size, TickType_t ticks_to_wait = portMAX_DELAY)
    {
        KIT_LOGD("write: %zu", size);
        size_t bytes_written = 0;
        if (i2s_write((i2s_port_t)cfg.i2s_num, src, size, &bytes_written, ticks_to_wait) != ESP_OK) {
            KIT_LOGE("i2s_write");
        }
        return bytes_written;
    }

    /// Reads the audio data via i2s from the ADC
    size_t read(void* dest, size_t size, TickType_t ticks_to_wait = portMAX_DELAY)
    {
        KIT_LOGD("read: %zu", size);
        size_t bytes_read = 0;
        if (i2s_read((i2s_port_t)cfg.i2s_num, dest, size, &bytes_read, ticks_to_wait) != ESP_OK) {
            KIT_LOGE("i2s_read");
        }
        return bytes_read;
    }

#endif

    /// Just update the sample rate
    bool setSampleRate(audio_hal_iface_samples_t sample_rate)
    {
        KIT_LOGD("setSampleRate");
        bool result = true;
        // update values
        audio_hal_conf.i2s_iface.samples = cfg.sample_rate = sample_rate;

        // apply new value
        if (is_active) {
            // esp_err_t audio_hal_codec_iface_config(audio_hal_handle_t audio_hal,
            // audio_hal_codec_mode_t mode, audio_hal_codec_i2s_iface_t *iface)
            if (audio_hal_codec_iface_config(
                    hal_handle, audio_hal_conf.codec_mode, &audio_hal_conf.i2s_iface)
                != ESP_OK) {
                KIT_LOGE("audio_hal_ctrl_codec");
                result = false;
            }
#if AUDIOKIT_ESP32_I2S && defined(ESP32)
            // update I2S
            if (cfg.i2s_active) {
                if (i2s_set_sample_rates((i2s_port_t)cfg.i2s_num, cfg.sampleRate()) != ESP_OK) {
                    KIT_LOGE("i2s_set_sample_rates");
                    result = false;
                }
            }
#endif
        }

        return result;
    }

    /**
     * @brief  Get the gpio number for auxin detection
     *
     * @return  -1      non-existent
     *          Others  gpio number
     */
    int8_t pinAuxin() { return driver.get_auxin_detect_gpio(); }

    /**
     * @brief  Get the gpio number for headphone detection
     *
     * @return  -1      non-existent
     *          Others  gpio number
     */
    int8_t pinHeadphoneDetect() { return driver.get_headphone_detect_gpio(); }

    /**
     * @brief  Get the gpio number for PA enable
     *
     * @return  -1      non-existent
     *          Others  gpio number
     */
    int8_t pinPaEnable() { return driver.get_pa_enable_gpio(); }

    /**
     * @brief  Get the gpio number for adc detection
     *
     * @return  -1      non-existent
     *          Others  gpio number
     */
    int8_t pinAdcDetect() { return driver.get_adc_detect_gpio(); }

    /**
     * @brief  Get the mclk gpio number of es7243
     *
     * @return  -1      non-existent
     *          Others  gpio number
     */
    int8_t pinEs7243Mclk() { return driver.get_es7243_mclk_gpio(); }

    /**
     * @brief  Get the record-button id for adc-button
     *
     * @return  -1      non-existent
     *          Others  button id
     */
    int8_t pinInputRec() { return driver.get_input_rec_id(); }

    /**
     * @brief  Get the number for mode-button
     *
     * @return  -1      non-existent
     *          Others  number
     */
    int8_t pinInputMode() { return driver.get_input_mode_id(); }

    /**
     * @brief Get number for set function
     *
     * @return -1       non-existent
     *         Others   number
     */
    int8_t pinInputSet() { return driver.get_input_set_id(); };

    /**
     * @brief Get number for play function
     *
     * @return -1       non-existent
     *         Others   number
     */
    int8_t pinInputPlay() { return driver.get_input_play_id(); }

    /**
     * @brief number for volume up function
     *
     * @return -1       non-existent
     *         Others   number
     */
    int8_t pinVolumeUp() { return driver.get_input_volup_id(); }

    /**
     * @brief Get number for volume down function
     *
     * @return -1       non-existent
     *         Others   number
     */
    int8_t pinVolumeDown() { return driver.get_input_voldown_id(); }

    /**
     * @brief Get reset codec gpio number
     *
     * @return -1       non-existent
     *        Others    gpio number
     */
    int8_t pinResetCodec() { return driver.get_reset_codec_gpio(); }

    /**
     * @brief Get  gpio number to reset the board
     *
     * @return -1       non-existent
     *         Others   gpio number
     */
    int8_t pinResetBoard() { return driver.get_reset_board_gpio(); }

    /**
     * @brief Get gpio number for green led
     *
     * @return -1       non-existent
     *         Others   gpio number
     */
    int8_t pinGreenLed() { return driver.get_green_led_gpio(); }

    /**
     * @brief Get blue led gpio number
     *
     * @return -1       non-existent
     *         Others   gpio number
     */
    int8_t pinBlueLed() { return driver.get_blue_led_gpio(); }

    /**
     * @brief SPI CS Pin for SD Drive
     * 
     * @return int8_t 
     */
    int8_t pinSpiCs() { return cfg.pins.sd_cs; }


    /**
     * @brief Activates/deactivates the speaker amplifier output
     * This is working only if the driver is supporting the functionality
     * @param active
     */
    void setSpeakerActive(bool active)
    {
        int paPin = driver.get_pa_enable_gpio();
        if (paPin > 0) {
            digitalWrite(paPin, active);
        } else {
            KIT_LOGW("setSpeakerActive not supported");
        }
    }

    /**
     * @brief Switch off the PA if the headphone in plugged in
     * and switch it on again if the headphone is unplugged
     *
     */
    static void actionHeadphoneDetection()
    {
        if (selfAudioKit->config().pins.detect_headphone > 0) {

            // detect changes
            bool isConnected = selfAudioKit->headphoneStatus();
            if (selfAudioKit->headphoneIsConnected != isConnected) {
                selfAudioKit->headphoneIsConnected = isConnected;

                // update if things have stabilized
                bool powerActive = !isConnected;
                KIT_LOGW("Headphone jack has been %s", isConnected ? "inserted" : "removed");
                selfAudioKit->setSpeakerActive(powerActive);
            }
        }
        yield();
    }

    /**
     * @brief Returns true if the headphone was detected
     *
     * @return true
     * @return false
     */
    bool headphoneStatus()
    {
        int headphonePin = config().pins.detect_headphone;
        return headphonePin > 0 ? !digitalRead(headphonePin) : false;
    }

    /// checks if the SD is active
    bool isSDActive() { return cfg.sd_active; }

    void log_i2c() { KIT_LOGI("i2c_sda %d, i2c_scl: %d", cfg.pins.i2c_sda, cfg.pins.i2c_scl); }

protected:
    bool is_active = false;
    AudioKitConfig cfg;
    audio_hal_codec_config_t audio_hal_conf;
    audio_hal_handle_t hal_handle = 0;
    audio_hal_codec_i2s_iface_t iface;
    bool headphoneIsConnected = false;
    unsigned long speakerChangeTimeout = 0;
    board_driver driver;
#if AUDIOKIT_SETUP_SD
    SPIClass* p_spi = &AUDIOKIT_SD_SPI;
#endif

    void setupDriverPins() { set_driver_pins(driver); }

    /**
     * @brief Setup the headphone detection
     */
    void setupHeadphoneDetection()
    {
        int8_t paPin = cfg.pins.enable_pa;
        if (paPin != -1) {
            int8_t headphonePin = cfg.pins.detect_headphone;
            if (headphonePin != -1) {
                pinMode(headphonePin, INPUT_PULLUP);
                pinMode(paPin, OUTPUT);
                KIT_LOGI("headphone detection is active");
            } else {
                KIT_LOGI("headphone detection not supported");
            }
        } else {
            KIT_LOGI("headphone detection: PA not supported");
        }
    }

#if defined(AUDIOKIT_SETUP_SD)
    /**
     * @brief Setup the SPI so that we can access the SD Drive
     */
    void setupSPI()
    {
//  I assume this is valid for all AudioKits!
#if AUDIOKIT_SETUP_SD == 1
        if (cfg.sd_active) {
            int spi_cs_pin = cfg.pins.sd_cs;
            if (spi_cs_pin != -1){
                KIT_LOGI("SPI: cs: %d", spi_cs_pin);
                pinMode(spi_cs_pin, OUTPUT);
                digitalWrite(spi_cs_pin, HIGH);
            }
#ifdef ESP32
            KIT_LOGI("clk: %d, miso: %d, mosi: %d",cfg.pins.sd_clk, cfg.pins.sd_miso, cfg.pins.sd_mosi);
            p_spi->begin(cfg.pins.sd_clk, cfg.pins.sd_miso, cfg.pins.sd_mosi, cfg.pins.sd_cs);
#else
            p_spi->begin();
#endif
        }
#else
#if defined(ARDUINO)
#warning "SPI initialization for the SD drive not supported - you might need to take care of this yourself"
#endif
        cfg.sd_active = false;
#endif
    }
#endif

    bool setupI2S()
    {
#if AUDIOKIT_ESP32_I2S && defined(ESP32)
        KIT_LOGD("setupI2S");
        // setup i2s driver - with no queue
        i2s_config_t i2s_config = cfg.i2sConfig();
        if (i2s_driver_install((i2s_port_t)cfg.i2s_num, &i2s_config, 0, NULL) != ESP_OK) {
            KIT_LOGE("i2s_driver_install");
            return false;
        }

        // define i2s pins
        i2s_pin_config_t pin_config = cfg.i2sPins();
        KIT_LOGI("i2s_set_pin");

        KIT_LOGI("- bck_io_num: %d", pin_config.bck_io_num);
        KIT_LOGI("- ws_io_num: %d", pin_config.ws_io_num);
        KIT_LOGI("- data_out_num: %d", pin_config.data_out_num);
        KIT_LOGI("- data_in_num: %d", pin_config.data_in_num);
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 4, 0)
        KIT_LOGI("- mck_io_num: %d", pin_config.mck_io_num);
#endif

        if (i2s_set_pin((i2s_port_t)cfg.i2s_num, &pin_config) != ESP_OK) {
            KIT_LOGE("i2s_set_pin");
            return false;
        }

#endif
        return true;
    }
};

} // namespace

using namespace audiokit;
