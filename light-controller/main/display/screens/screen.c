#include "screen.h"

#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "screen_display_calibration.h"
#include "screen_main.h"
#include "screen_types.h"

typedef struct screen_t {
	screen_code_t active_screen;
	screen_code_t previous_screen;

	bool is_locked;

	screen_handle_t active_screen_handle;

	screen_handle_t screen_main;
	screen_handle_t screen_display_calibration;

} screen_t;

static screen_t screen;

static void screen_display_active(void);

void screen_init(void) {
	screen.active_screen = SCREEN_CODE_MAIN;
	screen.previous_screen = SCREEN_CODE_MAIN;

	screen.screen_main = screen_main_init();
	screen.screen_display_calibration = screen_display_calibration_init();

	screen_display_active();
}

void screen_manage(void) {
	if(screen.active_screen_handle.manage_callback) {
		screen.active_screen_handle.manage_callback();
	}
}

void screen_display(screen_code_t screen_code) {
	if((screen.is_locked == false) && (screen_code != screen.active_screen)) {
		screen.previous_screen = screen.active_screen;

		screen.active_screen = screen_code;

		screen_display_active();
	}
}

void screen_display_previous(void) { screen_display(screen.previous_screen); }

void screen_set_lock(bool is_locked) { screen.is_locked = is_locked; }

static void screen_display_active(void) {
	lvgl_port_lock(0);

	bool screen_found = true;
	screen_handle_t new_screen_handle = {0};

	switch(screen.active_screen) {
		case SCREEN_CODE_MAIN: {
			new_screen_handle = screen.screen_main;
			break;
		}
		case SCREEN_CODE_CALIBRATION: {
			new_screen_handle = screen.screen_display_calibration;
			break;
		}
		default: {
			screen_found = false;
			break;
		}
	}

	if(screen_found) {
		lv_scr_load(new_screen_handle.screen);
		screen.active_screen_handle = new_screen_handle;
	}

	lvgl_port_unlock();
}
