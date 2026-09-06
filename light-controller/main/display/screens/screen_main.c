#include "screen_main.h"

typedef struct screen_main_t {
	lv_obj_t* screen;
	lv_obj_t* label;
} screen_main_t;

static screen_main_t screen_main;

screen_handle_t screen_main_init(void) {
	lvgl_port_lock(0);

	lv_obj_t* screen = lv_obj_create(NULL);
	lv_obj_remove_style_all(screen);
	lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label = lv_label_create(screen);
	lv_label_set_text(label, "Main screen");
	lv_obj_set_style_text_color(label, lv_color_white(), 0);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

	screen_main.screen = screen;
	screen_main.label = label;

	lvgl_port_unlock();

	const screen_handle_t handle = {.screen = screen_main.screen};
	return handle;
}
