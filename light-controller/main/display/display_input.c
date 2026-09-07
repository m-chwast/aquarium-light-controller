#include "display_input.h"

#include "display_calibration.h"

typedef struct display_input_t {
	int x;
	int y;
	bool is_pressed;
} display_input_t;

static display_input_t display_input;

static void display_input_set_lock(bool is_locked);

display_input_data_t display_input_get_data(void) {
	display_input_data_t data = {0};

	if(display_input.is_pressed) {
		display_input_set_lock(true);

		const display_calibration_point_t raw_point = {.x = display_input.x,
													   .y = display_input.y};

		display_input_set_lock(false);

		const display_calibration_point_t calibrated_point =
			display_calibration_get_calibrated_coordinates(raw_point);

		data.x = calibrated_point.x;
		data.y = calibrated_point.y;
		data.is_pressed = true;
	}

	return data;
}

// data before calibration
display_input_data_t display_input_get_data_raw(void) {
	display_input_data_t data = {0};

	if(display_input.is_pressed) {
		display_input_set_lock(true);
		data.x = display_input.x;
		data.y = display_input.y;
		data.is_pressed = true;
		display_input_set_lock(false);
	}

	return data;
}

static void display_input_set_lock(bool is_locked) {
	if(is_locked) {
	}
	else {
	}
}
