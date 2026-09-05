#include "console.h"

#include "esp_log.h"
#include "rtos.h"

#define CONSOLE_TAG "CONSOLE"

static void console_task_handler(void* arg);

static rtos_task_t console_task;

void console_init(void) {
	ESP_LOGI(CONSOLE_TAG, "Initializing");
	console_task =
		rtos_create_task(console_task_handler, CONSOLE_TAG,
						 RTOS_TASK_STACK_SIZE_2KB, RTOS_PRIORITY_LOW);

	ESP_LOGI(CONSOLE_TAG, "Init ok");
}

void console_task_handler(void* arg) {
	while(true) {
		rtos_delay_ms(1000);
	}
}
