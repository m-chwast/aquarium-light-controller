#include <stdio.h>

#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BLINK_GPIO GPIO_NUM_8

static void blink_task(void* arg);
static void lcd_task(void* arg);

void app_main(void) {
	xTaskCreate(&blink_task, "BLINK", 1024, NULL, 5, NULL);
	xTaskCreate(&lcd_task, "LCD", 2048, NULL, 5, NULL);
}

static void lcd_task(void* arg) {
	while(1) {
		ESP_LOGI("LCD", "Hello, LCD!");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static void blink_task(void* arg) {
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	while(1) {
		gpio_set_level(BLINK_GPIO, 1);
		vTaskDelay(pdMS_TO_TICKS(1000));

		gpio_set_level(BLINK_GPIO, 0);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
