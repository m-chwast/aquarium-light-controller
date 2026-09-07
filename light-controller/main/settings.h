#pragma once

#include <stdbool.h>

typedef enum settings_elem_t {
	// ints
	SETTINGS_ELEM_TYPEBEGIN_INT,

	SETTINGS_ELEM_DISPLAY_CALIB_X1,
	SETTINGS_ELEM_DISPLAY_CALIB_Y1,
	SETTINGS_ELEM_DISPLAY_CALIB_X2,
	SETTINGS_ELEM_DISPLAY_CALIB_Y2,
	SETTINGS_ELEM_DISPLAY_CALIB_X3,
	SETTINGS_ELEM_DISPLAY_CALIB_Y3,
	SETTINGS_ELEM_DISPLAY_CALIB_X4,
	SETTINGS_ELEM_DISPLAY_CALIB_Y4,
	SETTINGS_ELEM_DISPLAY_CALIB_X5,
	SETTINGS_ELEM_DISPLAY_CALIB_Y5,

	SETTINGS_ELEM_TYPEEND_INT,

	// booleans
	SETTINGS_ELEM_TYPEBEGIN_BOOL,

	SETTINGS_ELEM_DISPLAY_CALIB_IS_PERFORMED,

	SETTINGS_ELEM_TYPEEND_BOOL,

	SETTINGS_ELEM_COUNT
} settings_elem_t;

void settings_init(void);

void settings_set_int(settings_elem_t elem, int value);
int settings_get_int(settings_elem_t elem);

void settings_set_bool(settings_elem_t elem, bool value);
bool settings_get_bool(settings_elem_t elem);
