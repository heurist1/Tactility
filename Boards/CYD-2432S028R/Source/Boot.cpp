#include "YellowResConfig.h"
#include "TactilityCore.h"
#include "hal/YellowResTouchConstants.h"
#include "hal/YellowResDisplayConstants.h"
#include <driver/spi_common.h>

#define TAG "yellow_res_bootstrap"

static bool init_touch() { // TOUCH
    TT_LOG_I(TAG, LOG_MESSAGE_SPI_INIT_START_FMT, YELLOW_RES_TOUCH_PORT);

    const spi_bus_config_t bus_config = {
        .mosi_io_num = YELLOW_RES_TOUCH_PIN_MOSI,
        .miso_io_num = YELLOW_RES_TOUCH_PIN_MISO,
        .sclk_io_num = YELLOW_RES_TOUCH_PIN_SCLK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = YELLOW_RES_TOUCH_TRANSACTION_LIMIT
   };

    if (spi_bus_initialize(YELLOW_RES_TOUCH_PORT, &bus_config, SPI_DMA_CH_AUTO) != ESP_OK) {
        TT_LOG_E(TAG, LOG_MESSAGE_SPI_INIT_FAILED_FMT, YELLOW_RES_TOUCH_PORT);
        return false;
    }

    return true;
}

static bool init_disp() { // DISPLAY
    TT_LOG_I(TAG, LOG_MESSAGE_SPI_INIT_START_FMT, YELLOW_RES_LCD_PORT);

    const spi_bus_config_t bus_config = {
        .mosi_io_num = YELLOW_RES_DISP_PIN_MOSI,
        .miso_io_num = GPIO_NUM_NC,
        .sclk_io_num = YELLOW_RES_DISP_PIN_SCLK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = YELLOW_RES_DISP_TRANSACTION_LIMIT
   };

    if (spi_bus_initialize(YELLOW_RES_LCD_PORT, &bus_config, SPI_DMA_CH_AUTO) != ESP_OK) {
        TT_LOG_E(TAG, LOG_MESSAGE_SPI_INIT_FAILED_FMT, YELLOW_RES_LCD_PORT);
        return false;
    }

    return true;
}

static bool init_card() { // SDCARD
    TT_LOG_I(TAG, LOG_MESSAGE_SPI_INIT_START_FMT, SPI3_HOST);

    const spi_bus_config_t bus_config = {
        .mosi_io_num = YELLOW_RES_CARD_PIN_MOSI,
        .miso_io_num = YELLOW_RES_CARD_PIN_MISO,
        .sclk_io_num = YELLOW_RES_CARD_PIN_SCLK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .data4_io_num = 0,
        .data5_io_num = 0,
        .data6_io_num = 0,
        .data7_io_num = 0,
        .max_transfer_sz = YELLOW_RES_CARD_TRANSACTION_LIMIT,
        .flags = 0,
        .isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO,
        .intr_flags = 0
    };

    if (spi_bus_initialize(SPI3_HOST, &bus_config, SPI_DMA_CH_AUTO) != ESP_OK) {
        TT_LOG_E(TAG, LOG_MESSAGE_SPI_INIT_FAILED_FMT, SPI3_HOST);
        return false;
    }

    return true;
}

bool yellow_res_boot() {
    return  init_disp() && init_touch() /*&& init_spi3()*/;
}
