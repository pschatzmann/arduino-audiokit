/**
 * @file audio_gpio.h
 * @author Phil Schatzmann
 * @brief GPIO related functionality
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#ifdef ESP32
#include "driver/gpio.h"
#else
#include "audio_hal/audio_types.h"
#include "audio_hal/audio_error.h"
#define I2S_NUM_0 0
#define I2S_NUM_1 1
#define I2S_NUM_MAX I2S_NUM_1
#define I2C_NUM_0 0
#define I2C_NUM_1 1
#define GPIO_MODE_OUTPUT 1 
#define GPIO_MODE_INPUT 0
#define BIT64(nr)               (1ULL << (nr))
#define BIT(nr)                 (1 << (nr))

// Suppress the following macros
#define PIN_FUNC_SELECT(a,b)
#define WRITE_PERI_REG(a,b)

#ifdef __cplusplus
extern "C" {
#endif

typedef int i2s_port_t;
typedef int i2c_port_t;
typedef uint32_t TickType_t;

typedef enum {
    GPIO_NUM_NC = -1,    /*!< Use to signal not connected to S/W */
    GPIO_NUM_0 = 0,     /*!< GPIO0, input and output */
    GPIO_NUM_1 = 1,     /*!< GPIO1, input and output */
    GPIO_NUM_2 = 2,     /*!< GPIO2, input and output */
    GPIO_NUM_3 = 3,     /*!< GPIO3, input and output */
    GPIO_NUM_4 = 4,     /*!< GPIO4, input and output */
    GPIO_NUM_5 = 5,     /*!< GPIO5, input and output */
    GPIO_NUM_6 = 6,     /*!< GPIO6, input and output */
    GPIO_NUM_7 = 7,     /*!< GPIO7, input and output */
    GPIO_NUM_8 = 8,     /*!< GPIO8, input and output */
    GPIO_NUM_9 = 9,     /*!< GPIO9, input and output */
    GPIO_NUM_10 = 10,   /*!< GPIO10, input and output */
    GPIO_NUM_11 = 11,   /*!< GPIO11, input and output */
    GPIO_NUM_12 = 12,   /*!< GPIO12, input and output */
    GPIO_NUM_13 = 13,   /*!< GPIO13, input and output */
    GPIO_NUM_14 = 14,   /*!< GPIO14, input and output */
    GPIO_NUM_15 = 15,   /*!< GPIO15, input and output */
    GPIO_NUM_16 = 16,   /*!< GPIO16, input and output */
    GPIO_NUM_17 = 17,   /*!< GPIO17, input and output */
    GPIO_NUM_18 = 18,   /*!< GPIO18, input and output */
    GPIO_NUM_19 = 19,   /*!< GPIO19, input and output */
    GPIO_NUM_20 = 20,   /*!< GPIO20, input and output */
    GPIO_NUM_21 = 21,   /*!< GPIO21, input and output */
    GPIO_NUM_22 = 22,   /*!< GPIO22, input and output */
    GPIO_NUM_23 = 23,   /*!< GPIO23, input and output */
    GPIO_NUM_25 = 25,   /*!< GPIO25, input and output */
    GPIO_NUM_26 = 26,   /*!< GPIO26, input and output */
    GPIO_NUM_27 = 27,   /*!< GPIO27, input and output */
    GPIO_NUM_28 = 28,   /*!< GPIO28, input and output */
    GPIO_NUM_29 = 29,   /*!< GPIO29, input and output */
    GPIO_NUM_30 = 30,   /*!< GPIO30, input and output */
    GPIO_NUM_31 = 31,   /*!< GPIO31, input and output */
    GPIO_NUM_32 = 32,   /*!< GPIO32, input and output */
    GPIO_NUM_33 = 33,   /*!< GPIO33, input and output */
    GPIO_NUM_34 = 34,   /*!< GPIO34, input mode only */
    GPIO_NUM_35 = 35,   /*!< GPIO35, input mode only */
    GPIO_NUM_36 = 36,   /*!< GPIO36, input mode only */
    GPIO_NUM_37 = 37,   /*!< GPIO37, input mode only */
    GPIO_NUM_38 = 38,   /*!< GPIO38, input mode only */
    GPIO_NUM_39 = 39,   /*!< GPIO39, input mode only */
    GPIO_NUM_MAX,
/** @endcond */
} gpio_num_t;

typedef enum{
    I2C_MODE_SLAVE = 0,   /*!< I2C slave mode */
    I2C_MODE_MASTER,      /*!< I2C master mode */
    I2C_MODE_MAX,
} i2c_mode_t;

typedef enum {
    GPIO_INTR_DISABLE = 0,     /*!< Disable GPIO interrupt                             */
    GPIO_INTR_POSEDGE = 1,     /*!< GPIO interrupt type : rising edge                  */
    GPIO_INTR_NEGEDGE = 2,     /*!< GPIO interrupt type : falling edge                 */
    GPIO_INTR_ANYEDGE = 3,     /*!< GPIO interrupt type : both rising and falling edge */
    GPIO_INTR_LOW_LEVEL = 4,   /*!< GPIO interrupt type : input low level trigger      */
    GPIO_INTR_HIGH_LEVEL = 5,  /*!< GPIO interrupt type : input high level trigger     */
    GPIO_INTR_MAX,
} gpio_int_type_t;


/// I2C pins
typedef struct{
    i2c_mode_t mode;     /*!< I2C mode */
    gpio_num_t sda_io_num;      /*!< GPIO number for I2C sda signal */
    gpio_num_t scl_io_num;      /*!< GPIO number for I2C scl signal */
    bool sda_pullup_en;  /*!< Internal GPIO pull mode for I2C sda signal*/
    bool scl_pullup_en;  /*!< Internal GPIO pull mode for I2C scl signal*/

    union {
        struct {
            uint32_t clk_speed;      /*!< I2C clock frequency for master mode, (no higher than 1MHz for now) */
        } master;                    /*!< I2C master config */
        struct {
            uint8_t addr_10bit_en;   /*!< I2C 10bit address mode enable for slave mode */
            uint16_t slave_addr;     /*!< I2C address for slave mode */
            uint32_t maximum_speed;  /*!< I2C expected clock speed from SCL. */
        } slave;                     /*!< I2C slave config */
    };
    uint32_t clk_flags;              /*!< Bitwise of ``I2C_SCLK_SRC_FLAG_**FOR_DFS**`` for clk source choice*/
} i2c_config_t;

/// I2S Pins
typedef struct {
    gpio_num_t bck_io_num;
    gpio_num_t ws_io_num;
    gpio_num_t data_out_num;
    gpio_num_t data_in_num;
} i2s_pin_config_t;

// SPI Configuration
typedef struct {
    gpio_num_t mosi_io_num;    ///< GPIO pin for Master Out Slave In (=spi_d) signal, or -1 if not used.
    gpio_num_t miso_io_num;    ///< GPIO pin for Master In Slave Out (=spi_q) signal, or -1 if not used.
    gpio_num_t sclk_io_num;      ///< GPIO pin for SPI Clock signal, or -1 if not used.
    gpio_num_t quadwp_io_num;
    gpio_num_t quadhd_io_num;
} spi_bus_config_t;

/// SPI device configuration
typedef struct {
    int spics_io_num;               ///< CS GPIO pin for this device, or -1 if not used
} spi_device_interface_config_t;

typedef enum {
    TOUCH_PAD_NUM0 = 0, /*!< Touch pad channel 0 is GPIO4 */
    TOUCH_PAD_NUM1,    /*!< Touch pad channel 0 is GPIO0 */
    TOUCH_PAD_NUM2,    /*!< Touch pad channel 0 is GPIO2 */
    TOUCH_PAD_NUM3,    /*!< Touch pad channel 0 is GPIO15 */
    TOUCH_PAD_NUM4,    /*!< Touch pad channel 0 is GPIO13 */
    TOUCH_PAD_NUM5,    /*!< Touch pad channel 0 is GPIO12 */
    TOUCH_PAD_NUM6,    /*!< Touch pad channel 0 is GPIO14 */
    TOUCH_PAD_NUM7,    /*!< Touch pad channel 0 is GPIO27*/
    TOUCH_PAD_NUM8,    /*!< Touch pad channel 0 is GPIO33*/
    TOUCH_PAD_NUM9,    /*!< Touch pad channel 0 is GPIO32*/
    TOUCH_PAD_MAX,
} touch_pad_t;



typedef struct {
    uint64_t pin_bit_mask;          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
    int mode;               /*!< GPIO mode: set input/output mode                     */
    int pull_up_en;       /*!< GPIO pull-up                                         */
    int pull_down_en;   /*!< GPIO pull-down                                       */
    int intr_type;      /*!< GPIO interrupt type                                  */
} gpio_config_t;

typedef enum {
    GPIO_PULLUP_ONLY,               /*!< Pad pull up            */
    GPIO_PULLDOWN_ONLY,             /*!< Pad pull down          */
    GPIO_PULLUP_PULLDOWN,           /*!< Pad pull up + pull down*/
    GPIO_FLOATING,                  /*!< Pad floating           */
} gpio_pull_mode_t;


esp_err_t gpio_config(const gpio_config_t *pGPIOConfig);
esp_err_t gpio_pad_select_gpio(gpio_num_t);
esp_err_t gpio_set_direction(gpio_num_t, int);
esp_err_t gpio_set_level(gpio_num_t, int);
esp_err_t vTaskDelay(TickType_t delay);

#ifdef __cplusplus
}
#endif


#endif