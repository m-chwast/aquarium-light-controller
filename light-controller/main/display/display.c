#include "display.h"

#include "display_ll.h"
#include "display_lvgl.h"
#include "display_request.h"
#include "rtos.h"

#define DISPLAY_TAG "DISPLAY"

static rtos_task_t display_task;

static void display_task_handler(void* arg);

void display_init(void) {
	display_ll_init();
	display_lvgl_init();
	display_request_init();

	display_task =
		rtos_create_task(display_task_handler, DISPLAY_TAG,
						 RTOS_TASK_STACK_SIZE_2KB, RTOS_PRIORITY_NORMAL);
}

static void display_task_handler(void* arg) {
	while(true) {
		rtos_delay_ms(33);
	}
}
