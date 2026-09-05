#include "display_request.h"

#include "esp_log.h"
#include "rtos.h"

#define DISPLAY_REQUEST_TAG "DISPLAY_REQ"

#define DISPLAY_REQUEST_MAX_COUNT 10

static rtos_queue_t display_request_queue;

void display_request_init(void) {
	ESP_LOGI(DISPLAY_REQUEST_TAG, "Initializing");

	display_request_queue = rtos_create_queue(DISPLAY_REQUEST_MAX_COUNT,
											  sizeof(display_request_type_t));

	ESP_LOGI(DISPLAY_REQUEST_TAG, "Init ok");
}

void display_request_send(display_request_type_t request_type) {
	rtos_queue_send(display_request_queue, &request_type);
}

display_request_type_t display_request_receive(void) {
	display_request_type_t request_type = DISPLAY_REQUEST_TYPE_NONE;

	const bool is_any_request = !rtos_queue_is_empty(display_request_queue);
	if(is_any_request) {
		rtos_queue_receive(display_request_queue, &request_type);
	}

	return request_type;
}
