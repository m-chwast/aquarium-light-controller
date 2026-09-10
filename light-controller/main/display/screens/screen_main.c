#include "screen_main.h"

#include "bt/bt_request.h"

typedef struct screen_main_t {
	lv_obj_t* screen;
	lv_obj_t* label;

	lv_obj_t* btn_scan;
} screen_main_t;

static screen_main_t screen_main;

static void screen_main_event_handler(lv_event_t* event);

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

	lv_obj_t* btn1 = lv_btn_create(screen);

	// button top
	lv_obj_set_size(btn1, 120, 60);
	lv_obj_align_to(btn1, NULL, LV_ALIGN_TOP_MID, 0, 20);

	lv_obj_set_style_bg_color(btn1, lv_palette_main(LV_PALETTE_AMBER),
							  LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn1, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label1 = lv_label_create(btn1);
	lv_label_set_text(label1, "BLE Scan");
	lv_obj_center(label1);

	lv_obj_add_event_cb(btn1, screen_main_event_handler, LV_EVENT_ALL,
						NULL);

	lvgl_port_unlock();

	screen_main.screen = screen;
	screen_main.label = label;
	screen_main.btn_scan = btn1;

	const screen_handle_t handle = {.screen = screen_main.screen};
	return handle;
}

static void screen_main_event_handler(lv_event_t* event) {
	const lv_event_code_t code = lv_event_get_code(event);
	const lv_obj_t* target = lv_event_get_target(event);

	switch(code) {
		case LV_EVENT_CLICKED: {
			if(target == screen_main.btn_scan) {
				bt_request_send(BT_REQUEST_TYPE_START_SCAN);
			}
			break;
		}
		default: {
			break;
		}
	}
}