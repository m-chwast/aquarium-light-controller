idf_component_register(
    SRCS 
        "main.c"
        "main_app_entry.c"
        "rtos.c"
        "display.c"
        "display_ll.c"
        "display_lvgl.c"
    INCLUDE_DIRS "."
    REQUIRES 
        "esp_lcd"
        "esp_lcd_ili9341")
