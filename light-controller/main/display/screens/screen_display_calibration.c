#include "screen_display_calibration.h"

#include "display/display_calibration.h"

#define TARGET_SIZE 20

typedef struct screen_display_calibration_t {
	lv_obj_t* screen;
	lv_obj_t* targets[DISPLAY_CALIBRATION_POINT_COUNT];
	lv_obj_t* label;
} screen_display_calibration_t;

static screen_display_calibration_t screen_display_calibration;

static void screen_display_calibration_manage(void);
static lv_obj_t* screen_display_calibration_create_target(lv_obj_t* parent);

static void screen_display_calibration_show_target(unsigned index, int x,
												   int y);

screen_handle_t screen_display_calibration_init(void) {
	lvgl_port_lock(0);

	lv_obj_t* screen = lv_obj_create(NULL);
	lv_obj_remove_style_all(screen);
	lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label = lv_label_create(screen);
	lv_label_set_text(label, "Touch the targets to calibrate");
	lv_obj_set_style_text_color(label, lv_color_white(), 0);
	lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 40);

	/* Crosshair target */
	for(unsigned i = 0; i < DISPLAY_CALIBRATION_POINT_COUNT; i++) {
		screen_display_calibration.targets[i] =
			screen_display_calibration_create_target(screen);
	}

	screen_display_calibration.screen = screen;
	screen_display_calibration.label = label;

	lvgl_port_unlock();

	const screen_handle_t handle = {
		.screen = screen_display_calibration.screen,
		.manage_callback = screen_display_calibration_manage};
	return handle;
}

static lv_obj_t* screen_display_calibration_create_target(lv_obj_t* parent) {
	lv_obj_t* target = lv_obj_create(parent);
	lv_obj_remove_style_all(target);
	lv_obj_set_size(target, TARGET_SIZE, TARGET_SIZE);
	lv_obj_set_style_border_width(target, 2, 0);
	lv_obj_set_style_border_color(target, lv_color_white(), 0);
	lv_obj_set_style_radius(target, LV_RADIUS_CIRCLE, 0);

	return target;
}

static void screen_display_calibration_manage(void) {
	const bool show_results = display_calibration_should_show_results();

	if(!show_results) {
		const display_calibration_point_t target_point =
			display_calibration_get_current_target_point();

		screen_display_calibration_show_target(0, target_point.x,
											   target_point.y);
	}
	else {
		for(unsigned i = 0; i < DISPLAY_CALIBRATION_POINT_COUNT; i++) {
			display_calibration_point_t target_point = {0};
			display_calibration_get_target_coords(i, &target_point);
			screen_display_calibration_show_target(i, target_point.x,
												   target_point.y);
		}
	}
}

static void screen_display_calibration_show_target(unsigned index, int x,
												   int y) {
	lvgl_port_lock(0);

	if(index < DISPLAY_CALIBRATION_POINT_COUNT) {
		struct screen_display_calibration_t* cal = &screen_display_calibration;
		const int target_half_size = TARGET_SIZE / 2;
		const int target_x = x - target_half_size;
		const int target_y = y - target_half_size;
		lv_obj_set_pos(cal->targets[index], target_x, target_y);
	}

	lvgl_port_unlock();
}
