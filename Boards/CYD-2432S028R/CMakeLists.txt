idf_component_register(
    SRC_DIRS "Source" "Source/hal"
    INCLUDE_DIRS "Source"
    REQUIRES Tactility esp_lvgl_port esp_lcd_touch_xpt2046 espressif__esp_lcd_ili9341 driver vfs fatfs
)
