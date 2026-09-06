#include "display.h"

#include "display_ll.h"
#include "display_lvgl.h"
#include "display_request.h"
#include "rtos.h"
#include "screens/screen.h"

#define DISPLAY_TAG "DISPLAY"

static rtos_task_t display_task;

static void display_task_handler(void* arg);
static void display_process_request(display_request_type_t request_type);

void display_init(void) {
	display_ll_init();
	display_lvgl_init();
	display_request_init();

	screen_init();

	display_task =
		rtos_create_task(display_task_handler, DISPLAY_TAG,
						 RTOS_TASK_STACK_SIZE_2KB, RTOS_PRIORITY_NORMAL);
}

static void display_task_handler(void* arg) {
	while(true) {
		rtos_delay_ms(50);

		const display_request_type_t request_type = display_request_receive();
		display_process_request(request_type);
	}
}

static void display_process_request(display_request_type_t request_type) {
	switch(request_type) {
		case DISPLAY_REQUEST_TYPE_NONE: {
			break;
		}
		case DISPLAY_REQUEST_TYPE_CALIBRATION: {
			screen_set_lock(true);
			screen_display(SCREEN_CODE_CALIBRATION);
			break;
		}
		case DISPLAY_REQUEST_TYPE_CALIBRATION_ABORT: {
			screen_set_lock(false);
			screen_display_previous();
			break;
		}
	}
}
