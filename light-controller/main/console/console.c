#include "console.h"

#include <stdio.h>

#include "esp_log.h"
#include "rtos.h"

#define CONSOLE_TAG "CONSOLE"

static void console_task_handler(void* arg);

typedef struct console_t {
	rtos_task_t task;

	char buffer[1024];
	unsigned buffer_current_size;
	unsigned buffer_start_index;
} console_t;

static console_t console;

void console_init(void) {
	ESP_LOGI(CONSOLE_TAG, "Initializing");

	memset(&console, 0, sizeof(console));

	console.task =
		rtos_create_task(console_task_handler, CONSOLE_TAG,
						 RTOS_TASK_STACK_SIZE_4KB, RTOS_PRIORITY_LOW);

	ESP_LOGI(CONSOLE_TAG, "Init ok");
}

static void console_append_char(char c) {}

static void console_task_handler(void* arg) {
	while(true) {
		rtos_delay_ms(100);

		int new_data = getchar();
		while(new_data != EOF) {
			const char c = (char)new_data;
			console_append_char(c);

			ESP_LOGI(CONSOLE_TAG, "Received char: %c", c);

			new_data = getchar();
		}
	}
}
