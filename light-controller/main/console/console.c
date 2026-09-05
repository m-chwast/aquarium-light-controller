#include "console.h"

#include <stdio.h>

#include "esp_log.h"
#include "rtos.h"

#define CONSOLE_TAG "CONSOLE"

#define CONSOLE_BUFFER_SIZE 256

static void console_task_handler(void* arg);

typedef struct console_t {
	rtos_task_t task;

	rtos_queue_t rx_buffer;
	bool ready_to_parse;
	bool rx_overflow;
} console_t;

static console_t console;

void console_init(void) {
	ESP_LOGI(CONSOLE_TAG, "Initializing");

	console.rx_buffer = rtos_create_queue(CONSOLE_BUFFER_SIZE, sizeof(char));

	console.task =
		rtos_create_task(console_task_handler, CONSOLE_TAG,
						 RTOS_TASK_STACK_SIZE_4KB, RTOS_PRIORITY_LOW);

	ESP_LOGI(CONSOLE_TAG, "Init ok");
}

static void console_append_char(char c) {
	if(c == '\r') {
		c = '\n';
	}

	const unsigned free_space = rtos_queue_get_free_space(console.rx_buffer);
	if(free_space == 0) {
		// flush queue and mark the overflow
		console.rx_overflow = true;
		rtos_queue_reset(console.rx_buffer);
	}

	rtos_queue_send(console.rx_buffer, &c);

	if(c == '\n') {
		console.ready_to_parse = true;
	}
}

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
