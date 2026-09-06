#include "screen_display_calibration.h"

typedef struct screen_display_calibration_t {
	lv_obj_t* screen;
	lv_obj_t* target;
	lv_obj_t* label;
} screen_display_calibration_t;

static screen_display_calibration_t screen_display_calibration;

static void show_target(int x, int y);

screen_handle_t screen_display_calibration_init(void) {
	lvgl_port_lock(0);

	lv_obj_t* screen = lv_obj_create(NULL);
	lv_obj_remove_style_all(screen);
	lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label = lv_label_create(screen);
	lv_label_set_text(label, "Touch the targets to calibrate");
	lv_obj_set_style_text_color(label, lv_color_white(), 0);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

	/* Crosshair target */
	lv_obj_t* target = lv_obj_create(screen);
	lv_obj_remove_style_all(target);
	lv_obj_set_size(target, 20, 20);
	lv_obj_set_style_border_width(target, 2, 0);
	lv_obj_set_style_border_color(target, lv_color_white(), 0);
	lv_obj_set_style_radius(target, LV_RADIUS_CIRCLE, 0);

	screen_display_calibration.screen = screen;
	screen_display_calibration.label = label;
	screen_display_calibration.target = target;

	/* First calibration point: top-left */
	show_target(20, 20);

	lvgl_port_unlock();

	const screen_handle_t handle = {.screen =
										screen_display_calibration.screen};
	return handle;
}

static void show_target(int x, int y) {
	struct screen_display_calibration_t* cal = &screen_display_calibration;
	lv_obj_set_pos(cal->target, x - 10, y - 10);
}
