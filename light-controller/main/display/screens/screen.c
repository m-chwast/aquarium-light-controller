#include "screen.h"

#include "esp_lvgl_port.h"
#include "lvgl.h"

typedef struct screen_t {
	screen_code_t active_screen;
	screen_code_t previous_screen;

	bool is_locked;

	struct screen_calibration_t {
		lv_obj_t* screen;
		lv_obj_t* target;
	} screen_calibration;

	struct screen_main_t {
		lv_obj_t* screen;
		lv_obj_t* label;
	} screen_main;
} screen_t;

static screen_t screen;

static void screen_init_calib(void);
static void screen_init_main(void);

static void screen_display_active(void);

void screen_init(void) {
	screen.active_screen = SCREEN_CODE_MAIN;
	screen.previous_screen = SCREEN_CODE_MAIN;

	screen_init_main();
	screen_init_calib();

	screen_display_active();
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

static void show_target(int x, int y) {
	struct screen_calibration_t* cal = &screen.screen_calibration;
	lv_obj_set_pos(cal->target, x - 10, y - 10);
}

static void screen_init_calib(void) {
	lvgl_port_lock(0);

	struct screen_calibration_t* cal = &screen.screen_calibration;

	cal->screen = lv_obj_create(NULL);
	lv_obj_remove_style_all(cal->screen);
	lv_obj_set_style_bg_color(cal->screen, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(cal->screen, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label = lv_label_create(cal->screen);
	lv_label_set_text(label, "Touch the targets to calibrate");
	lv_obj_set_style_text_color(label, lv_color_white(), 0);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

	/* Crosshair target */
	cal->target = lv_obj_create(cal->screen);
	lv_obj_remove_style_all(cal->target);
	lv_obj_set_size(cal->target, 20, 20);
	lv_obj_set_style_border_width(cal->target, 2, 0);
	lv_obj_set_style_border_color(cal->target, lv_color_white(), 0);
	lv_obj_set_style_radius(cal->target, LV_RADIUS_CIRCLE, 0);

	/* First calibration point: top-left */
	show_target(20, 20);

	lvgl_port_unlock();
}

static void screen_init_main(void) {
	lvgl_port_lock(0);

	struct screen_main_t* main = &screen.screen_main;

	main->screen = lv_obj_create(NULL);
	lv_obj_remove_style_all(main->screen);
	lv_obj_set_style_bg_color(main->screen, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(main->screen, LV_OPA_COVER, LV_PART_MAIN);

	main->label = lv_label_create(main->screen);
	lv_label_set_text(main->label, "Main screen");
	lv_obj_set_style_text_color(main->label, lv_color_white(), 0);
	lv_obj_align(main->label, LV_ALIGN_CENTER, 0, 0);

	lvgl_port_unlock();
}

static void screen_display_active(void) {
	lvgl_port_lock(0);

	switch(screen.active_screen) {
		case SCREEN_CODE_MAIN: {
			lv_scr_load(screen.screen_main.screen);
			break;
		}
		case SCREEN_CODE_CALIBRATION: {
			lv_scr_load(screen.screen_calibration.screen);
			break;
		}
	}

	lvgl_port_unlock();
}
