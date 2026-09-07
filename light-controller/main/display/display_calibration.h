#pragma once

#include <stdbool.h>

typedef struct display_calibration_point_t {
	int x;
	int y;
} display_calibration_point_t;

void display_calibration_init(void);

void display_calibration_start(void);

void display_calibration_abort(void);

bool display_calibration_is_active(void);

display_calibration_point_t display_calibration_get_calibrated_coordinates(
	display_calibration_point_t raw_point);
