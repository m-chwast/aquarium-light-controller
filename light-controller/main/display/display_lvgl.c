#include "display_lvgl.h"

#include "display_input.h"
#include "display_ll.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lv_demos.h"
#include "lvgl.h"

static void display_lvgl_init_port(void);
static void display_lvgl_init_touch(void);
static lv_obj_t* display_lvgl_create_touch_cursor(void);
static void display_lvgl_draw_initial_screen(void);

static lv_display_t* display = NULL;
static lv_indev_t* indev = NULL;  // input device driver (Touch)

void display_lvgl_init(void) {
	display_lvgl_init_port();

	const unsigned lcd_width = display_ll_get_width();
	const unsigned lcd_height = display_ll_get_height();

	const esp_lcd_panel_io_handle_t io_handle = display_ll_get_io_handle();
	const esp_lcd_panel_handle_t panel_handle = display_ll_get_panel_handle();

	const lvgl_port_display_cfg_t display_cfg = {
		.io_handle = io_handle,
		.panel_handle = panel_handle,
		.buffer_size = lcd_width * 40,
		.double_buffer = true,
		.hres = lcd_width,
		.vres = lcd_height,
		.monochrome = false,
		.color_format = LV_COLOR_FORMAT_RGB565,
		.rotation =
			{
				.swap_xy = false,
				.mirror_x = false,
				.mirror_y = true,
			},
		.flags =
			{
				.buff_dma = true,
				.buff_spiram = false,
				.sw_rotate = false,
				.swap_bytes = true,
			},
	};

	display = lvgl_port_add_disp(&display_cfg);

	display_lvgl_init_touch();

	display_lvgl_draw_initial_screen();

	// after initial screen is drawn
	lv_indev_set_cursor(indev, display_lvgl_create_touch_cursor());
}

static void display_lvgl_init_port(void) {
	const lvgl_port_cfg_t lvgl_cfg = {
		.task_priority = 1,
		.task_stack = 6 * 1024,
		.task_affinity = -1,
		.task_max_sleep_ms = 500,
		.timer_period_ms = 15,
	};

	ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));
}

static void example_lvgl_touch_cb(lv_indev_t* indev, lv_indev_data_t* data) {
	uint16_t touchpad_x[1] = {0};
	uint16_t touchpad_y[1] = {0};
	uint8_t touchpad_cnt = 0;

	esp_lcd_touch_handle_t touch_pad = lv_indev_get_user_data(indev);
	esp_lcd_touch_read_data(touch_pad);
	/* Get coordinates */
	bool touchpad_pressed = esp_lcd_touch_get_coordinates(
		touch_pad, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);

	if(touchpad_cnt < 1) {
		touchpad_pressed = false;
	}

	const display_input_data_t input_data = {
		.x = touchpad_x[0],
		.y = touchpad_y[0],
		.is_pressed = touchpad_pressed,
	};
	display_input_provide_data(input_data);

	const display_input_data_t input_calibrated = display_input_get_data();

	if(touchpad_pressed) {
		data->point.x = input_calibrated.x;
		data->point.y = input_calibrated.y;
		data->state = LV_INDEV_STATE_PRESSED;
		ESP_LOGI("TOUCH", "Touch at (%d, %d)", data->point.x, data->point.y);
	}
	else {
		data->state = LV_INDEV_STATE_RELEASED;
	}
}

static void display_lvgl_init_touch(void) {
	indev = lv_indev_create();

	assert(indev);
	assert(display);

	lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

	lv_indev_set_display(indev, display);
	lv_indev_set_user_data(indev, display_ll_get_touch_handle());
	lv_indev_set_read_cb(indev, example_lvgl_touch_cb);
}

static lv_obj_t* display_lvgl_create_touch_cursor(void) {
	lv_obj_t* cursor = lv_obj_create(lv_screen_active());

	lv_obj_remove_style_all(cursor);
	lv_obj_set_size(cursor, 18, 18);
	lv_obj_set_style_radius(cursor, LV_RADIUS_CIRCLE, LV_PART_MAIN);
	lv_obj_set_style_bg_color(cursor, lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(cursor, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_border_width(cursor, 2, LV_PART_MAIN);
	lv_obj_set_style_border_color(cursor, lv_color_hex(0x000000), LV_PART_MAIN);
	lv_obj_add_flag(cursor, LV_OBJ_FLAG_HIDDEN);

	return cursor;
}

static void display_lvgl_draw_initial_screen_custom(void) {
	lvgl_port_lock(0);

	lv_obj_t* btn = lv_btn_create(lv_screen_active());

	// button top
	lv_obj_set_size(btn, 120, 60);
	lv_obj_align_to(btn, NULL, LV_ALIGN_TOP_MID, 0, 20);

	lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label = lv_label_create(btn);
	lv_label_set_text(label, "GREEN");
	lv_obj_center(label);

	// button middle
	lv_obj_t* btn2 = lv_btn_create(lv_screen_active());
	lv_obj_set_size(btn2, 120, 60);
	lv_obj_center(btn2);

	lv_obj_set_style_bg_color(btn2, lv_color_hex(0xFF0000), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn2, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label2 = lv_label_create(btn2);
	lv_label_set_text(label2, "RED");
	lv_obj_center(label2);

	// button bottom
	lv_obj_t* btn3 = lv_btn_create(lv_screen_active());
	lv_obj_set_size(btn3, 120, 60);
	lv_obj_align_to(btn3, NULL, LV_ALIGN_BOTTOM_MID, 0, -20);

	lv_obj_set_style_bg_color(btn3, lv_color_hex(0x0000FF), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(btn3, LV_OPA_COVER, LV_PART_MAIN);

	lv_obj_t* label3 = lv_label_create(btn3);
	lv_label_set_text(label3, "BLUE");
	lv_obj_center(label3);

	lvgl_port_unlock();
}

static void display_lvgl_draw_initial_screen(void) {
	display_lvgl_draw_initial_screen_custom();
}
