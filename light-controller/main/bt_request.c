#include "bt_request.h"

#include "esp_log.h"
#include "rtos.h"

#define BT_REQUEST_QUEUE_SIZE 10

typedef struct bt_request_t {
    rtos_queue_t queue;
} bt_request_t;

static bt_request_t bt_request;

void bt_request_init(void) {
    bt_request.queue = rtos_create_queue(BT_REQUEST_QUEUE_SIZE, sizeof(bt_request_type_t));
}

void bt_request_send(bt_request_type_t type) {
    rtos_queue_send(bt_request.queue, &type);
}

bt_request_type_t bt_request_receive(void) {
	bt_request_type_t request_type = BT_REQUEST_TYPE_NONE;

	const bool is_any_request = !rtos_queue_is_empty(bt_request.queue);
	if(is_any_request) {
		rtos_queue_receive(bt_request.queue, &request_type);
	}

	return request_type;
}
