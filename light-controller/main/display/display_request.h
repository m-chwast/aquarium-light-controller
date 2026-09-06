#pragma once

typedef enum display_request_type_t {
	DISPLAY_REQUEST_TYPE_NONE = 0,
	DISPLAY_REQUEST_TYPE_CALIBRATION,
	DISPLAY_REQUEST_TYPE_CALIBRATION_ABORT,
} display_request_type_t;

void display_request_init(void);

void display_request_send(display_request_type_t request_type);

display_request_type_t display_request_receive(void);
