#include "YellowResConfig.h"
#include "YellowResTouch.h"
#include "YellowResTouchConstants.h"
#include "Log.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_lcd_touch_xpt2046.h"
#include "esp_lcd_touch.h"
#include "esp_lvgl_port.h"

#define TAG "yellow_res_touch"

bool YellowResTouch::start(lv_display_t* display) {
    TT_LOG_I(TAG, "Starting");
    const esp_lcd_panel_io_spi_config_t touch_io_config = ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(YELLOW_RES_TOUCH_PIN_CS);
    TT_LOG_I(TAG, "2");

    if (esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)YELLOW_RES_TOUCH_PORT, &touch_io_config, &ioHandle) != ESP_OK) {
        TT_LOG_E(TAG, "Touch SPI init failed");
        return false;
    }
    TT_LOG_I(TAG, "3");

    esp_lcd_touch_config_t config = {
        .x_max = 240,
        .y_max = 320,
        .rst_gpio_num = GPIO_NUM_NC, //GPIO_NUM_25,
        .int_gpio_num = GPIO_NUM_NC, //GPIO_NUM_21,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 1,
            .mirror_y = 0,
        },
        .process_coordinates = nullptr,
        .interrupt_callback = nullptr,
        .user_data = nullptr,
        .driver_data = nullptr
    };

    TT_LOG_I(TAG, "4");

    if (esp_lcd_touch_new_spi_xpt2046(ioHandle, &config, &touchHandle) != ESP_OK) {
        TT_LOG_E(TAG, "Driver init failed");
        cleanup();
        return false;
    }

    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = display,
        .handle = touchHandle,
    };

    TT_LOG_I(TAG, "5");
	
    deviceHandle = lvgl_port_add_touch(&touch_cfg);
    if (deviceHandle == nullptr) {
        TT_LOG_E(TAG, "Adding touch failed");
        cleanup();
        return false;
    }

    TT_LOG_I(TAG, "Finished");
    return true;
}

bool YellowResTouch::stop() {
    cleanup();
    return true;
}

void YellowResTouch::cleanup() {
    if (deviceHandle != nullptr) {
        lv_indev_delete(deviceHandle);
        deviceHandle = nullptr;
    }

    if (touchHandle != nullptr) {
        esp_lcd_touch_del(touchHandle);
        touchHandle = nullptr;
    }

    if (ioHandle != nullptr) {
        esp_lcd_panel_io_del(ioHandle);
        ioHandle = nullptr;
    }
}
