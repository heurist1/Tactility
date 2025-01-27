#include "XPT2046_sw_spi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <rom/ets_sys.h>

// Define GPIO pins
static gpio_num_t TCLK = (gpio_num_t)0u; // Clock pin
static gpio_num_t TCS = (gpio_num_t)0u;  // Chip select pin
static gpio_num_t TDIN = (gpio_num_t)0u; // Data in (MOSI)
static gpio_num_t TDOUT = (gpio_num_t)0u; // Data out (MISO)
static uint16_t TIRQ = 0u;  // Touch interrupt pin (if needed)

static const char *TAG = "XPT2046";

// Function to initialize GPIO pins
void initPins(uint16_t tclk, uint16_t tcs, uint16_t tdin, uint16_t tdout, uint16_t tirq ) {
    TCLK = (gpio_num_t)tclk;
    TCS = (gpio_num_t)tcs;
    TDIN = (gpio_num_t)tdin;
    TDOUT = (gpio_num_t)tdout;
    TIRQ = tirq;

    gpio_config_t io_conf = {};

    // Configure TCLK, TCS, and TDIN as output
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << TCLK) | (1ULL << TCS) | (1ULL << TDIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Configure TDOUT as input
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << TDOUT);
    gpio_config(&io_conf);

    // Set initial states
    gpio_set_level(TCLK, 1); // Clock idle high
    gpio_set_level(TCS, 1);  // Deselect device
    gpio_set_level(TDIN, 0); // Default state
}

// Function to send and receive a byte via bit-banged SPI
uint8_t transferByte(uint8_t data) {
    uint8_t received = 0;
    for (int i = 7; i >= 0; i--) {
        // Write MOSI
        gpio_set_level(TDIN, (data >> i) & 1);

        // Pulse clock
        gpio_set_level(TCLK, 0);
        ets_delay_us(1); // Short delay

        // Read MISO
        received = (received << 1) | gpio_get_level(TDOUT);

        // Release clock
        gpio_set_level(TCLK, 1);
        ets_delay_us(1);
    }
    return received;
}

// Function to read a 12-bit value from the touch controller
uint16_t readTouch(uint8_t command) {
    uint16_t value = 0;

    gpio_set_level(TCS, 0); // Select the device

    transferByte(command); // Send command byte

    for (int i = 0; i < 12; i++) {
        // Pulse clock and read bits
        gpio_set_level(TCLK, 0);
        ets_delay_us(1);
        value = (value << 1) | gpio_get_level(TDOUT);
        gpio_set_level(TCLK, 1);
        ets_delay_us(1);
    }

    gpio_set_level(TCS, 1); // Deselect the device

    return value;
}

// Read X and Y coordinates from the touch controller
void readTouchCoordinates(uint16_t *x, uint16_t *y) {
    *x = readTouch(0x90); // Command for X position
    *y = readTouch(0xD0); // Command for Y position
}

