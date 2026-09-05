#include "console_cmd.h"

#include <string.h>

#include "console_cmd_handles.h"
#include "esp_log.h"

#define CONSOLE_CMD_TAG "CONSOLE_CMD"

static bool console_cmd_is_equal(const char* cmd, const char* expected_cmd);

void console_cmd_process(const char* cmd) {
	if(console_cmd_is_equal(cmd, "calib")) {
		console_cmd_handles_calib();
	}
	else {
		ESP_LOGW(CONSOLE_CMD_TAG, "Unknown command: %s", cmd);
	}
}

static bool console_cmd_is_equal(const char* cmd, const char* expected_cmd) {
	if(strcmp(cmd, expected_cmd) == 0) {
		return true;
	}
	else {
		return false;
	}
}
