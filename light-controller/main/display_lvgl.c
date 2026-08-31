#include "display_lvgl.h"

#include "display_ll.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"

static void display_lvgl_init_port(void);

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
		.rotation =
			{
				.swap_xy = false,
				.mirror_x = false,
				.mirror_y = false,
			},
		.flags =
			{
				.buff_dma = true,
				.buff_spiram = false,
				.sw_rotate = false,
			},
	};

	lv_display_t* display = lvgl_port_add_disp(&display_cfg);

	lvgl_port_lock(0);

	lv_obj_t* scr = lv_screen_active();

	lv_obj_set_style_bg_color(scr, lv_color_hex(0x0000FF), LV_PART_MAIN);

	lvgl_port_unlock();
}

static void display_lvgl_init_port(void) {
	const lvgl_port_cfg_t lvgl_cfg = {
		.task_priority = 4,
		.task_stack = 4096,
		.task_affinity = -1,
		.task_max_sleep_ms = 500,
		.timer_period_ms = 5,
	};

	ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));
}