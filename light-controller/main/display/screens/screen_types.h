#pragma once

#include "esp_lvgl_port.h"
#include "lvgl.h"

typedef void (*screen_manage_callback_t)(void);

typedef struct screen_handle_t {
	lv_obj_t* screen;
	screen_manage_callback_t manage_callback;
} screen_handle_t;
