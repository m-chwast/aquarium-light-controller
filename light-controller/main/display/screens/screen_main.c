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

	lv_obj_t* btn = lv_btn_create(screen);

	// button top
	lv_obj_set_size(btn, 120, 60);
	lv_obj_align_to(btn, NULL, LV_ALIGN_TOP_MID, 0, 20);

	lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label1 = lv_label_create(btn);
	lv_label_set_text(label1, "GREEN");
	lv_obj_center(label1);

	// button middle
	lv_obj_t* btn2 = lv_btn_create(screen);
	lv_obj_set_size(btn2, 120, 60);
	lv_obj_center(btn2);

	lv_obj_set_style_bg_color(btn2, lv_color_hex(0xFF0000), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn2, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label2 = lv_label_create(btn2);
	lv_label_set_text(label2, "RED");
	lv_obj_center(label2);

	// button bottom
	lv_obj_t* btn3 = lv_btn_create(screen);
	lv_obj_set_size(btn3, 120, 60);
	lv_obj_align_to(btn3, NULL, LV_ALIGN_BOTTOM_MID, 0, -20);

	lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000FF), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn3, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label3 = lv_label_create(btn3);
	lv_label_set_text(label3, "BLUE");
	lv_obj_center(label3);

	lvgl_port_unlock();

	screen_main.screen = screen;
	screen_main.label = label;

	const screen_handle_t handle = {.screen = screen_main.screen};
	return handle;
}
