#include "console_cmd_handles.h"

#include "esp_log.h"

#define CONSOLE_CMD_TAG "CONSOLE_CMD"

void console_cmd_handles_calib(void) {
	ESP_LOGI(CONSOLE_CMD_TAG, "Display calibration command received");
}
