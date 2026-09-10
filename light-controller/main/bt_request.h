#pragma once

typedef enum bt_request_type_t {
	BT_REQUEST_TYPE_NONE,
	BT_REQUEST_TYPE_START_SCAN,
} bt_request_type_t;

void bt_request_init(void);

void bt_request_send(bt_request_type_t type);

bt_request_type_t bt_request_receive(void);
