#pragma once

#include <stdbool.h>

#define DISPLAY_CALIBRATION_POINT_COUNT 5

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

// only for screen to display the target point
display_calibration_point_t display_calibration_get_current_target_point(void);
// only for screen to display the results
bool display_calibration_should_show_results(void);

void display_calibration_get_target_coords(unsigned index,
										   display_calibration_point_t* point);