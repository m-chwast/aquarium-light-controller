idf_component_register(
    SRCS 
        "main.c"
        "main_app_entry.cpp"
        "rtos.cpp"
        "display.cpp"
        "display_ll.c"
    INCLUDE_DIRS "."
    REQUIRES 
        "esp_lcd"
        "esp_lcd_ili9341")
