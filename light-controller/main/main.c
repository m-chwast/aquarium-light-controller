#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "periphs.h"
#include "main_app_entry.h"

#define TAG "main"

static void blink_task(void* arg);

void app_main(void) {
	xTaskCreate(&blink_task, "BLINK", 1024, NULL, 5, NULL);
	
    main_app_entry();

	while(1) {
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static void blink_task(void* arg) {
	gpio_set_direction(GPIO_BLINK, GPIO_MODE_OUTPUT);

	while(1) {
		gpio_set_level(GPIO_BLINK, 1);
		vTaskDelay(pdMS_TO_TICKS(1000));

		gpio_set_level(GPIO_BLINK, 0);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
