#pragma once

#include <stdbool.h>

typedef enum screen_code_t {
	SCREEN_CODE_MAIN,
	SCREEN_CODE_CALIBRATION,
} screen_code_t;

void screen_init(void);

void screen_manage(void);

void screen_display(screen_code_t screen_code);

void screen_display_previous(void);

void screen_set_lock(bool is_locked);
