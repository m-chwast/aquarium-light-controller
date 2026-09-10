#include "console_cmd_handles.h"

#include "bt/bt_request.h"
#include "display/display_request.h"
#include "esp_log.h"

#define CONSOLE_CMD_TAG "CONSOLE_CMD"

void console_cmd_handles_calib(void) {
	ESP_LOGI(CONSOLE_CMD_TAG, "Display calibration command received");
	display_request_send(DISPLAY_REQUEST_TYPE_CALIBRATION);
}

void console_cmd_handles_calib_abort(void) {
	ESP_LOGI(CONSOLE_CMD_TAG, "Display calibration abort command received");
	display_request_send(DISPLAY_REQUEST_TYPE_CALIBRATION_ABORT);
}

void console_cmd_handles_bt_scan(void) {
	ESP_LOGI(CONSOLE_CMD_TAG, "BT scan command received");
	bt_request_send(BT_REQUEST_TYPE_START_SCAN);
}
