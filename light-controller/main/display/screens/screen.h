#pragma once

typedef enum screen_code_t {
	SCREEN_CODE_MAIN,
	SCREEN_CODE_CALIBRATION,
} screen_code_t;

void screen_init(void);

void screen_set_active(screen_code_t screen_code);
