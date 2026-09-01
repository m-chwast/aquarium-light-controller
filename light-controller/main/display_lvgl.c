#include "display_lvgl.h"

#include "display_ll.h"
#include "esp_lvgl_port.h"
#include "lv_demos.h"
#include "lvgl.h"

static void display_lvgl_init_port(void);
static void display_lvgl_draw_initial_screen(void);

void display_lvgl_init(void) {
	display_lvgl_init_port();

	const unsigned lcd_width = display_ll_get_width();
	const unsigned lcd_height = display_ll_get_height();

	const esp_lcd_panel_io_handle_t io_handle = display_ll_get_io_handle();
	const esp_lcd_panel_handle_t panel_handle = display_ll_get_panel_handle();

	const lvgl_port_display_cfg_t display_cfg = {
		.io_handle = io_handle,
		.panel_handle = panel_handle,
		.buffer_size = lcd_width * 80,
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

	lv_display_t* display = lvgl_port_add_disp(&display_cfg);

	display_lvgl_draw_initial_screen();
}

static void display_lvgl_init_port(void) {
	const lvgl_port_cfg_t lvgl_cfg = {
		.task_priority =
			0,	// for now must be 0 to not trigger IDLE task watchdog
		.task_stack = 40960,
		.task_affinity = -1,
		.task_max_sleep_ms = 500,
		.timer_period_ms = 5,
	};

	ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));
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
	// lv_demo_benchmark();
}
