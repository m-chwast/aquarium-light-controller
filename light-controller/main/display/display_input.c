#include "display_input.h"

#include "display_calibration.h"
#include "rtos.h"

typedef struct display_input_t {
	display_input_data_t data;

	rtos_mutex_t mutex;
} display_input_t;

static display_input_t display_input;

static void display_input_set_lock(bool is_locked);

void display_input_init(void) {
	display_input.data.x = 0;
	display_input.data.y = 0;
	display_input.data.is_pressed = false;

	display_input.mutex = rtos_create_mutex();
}

display_input_data_t display_input_get_data(void) {
	display_input_data_t data = {0};

	if(display_input.data.is_pressed) {
		display_input_set_lock(true);

		const display_calibration_point_t raw_point = {
			.x = display_input.data.x, .y = display_input.data.y};

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

	if(display_input.data.is_pressed) {
		display_input_set_lock(true);
		data.x = display_input.data.x;
		data.y = display_input.data.y;
		data.is_pressed = true;
		display_input_set_lock(false);
	}

	return data;
}

bool display_input_is_pressed(void) {
	const bool is_pressed = display_input.data.is_pressed;
	return is_pressed;
}

void display_input_provide_data(display_input_data_t data) {
	rtos_mutex_lock(display_input.mutex);
	display_input.data = data;
	rtos_mutex_unlock(display_input.mutex);
}

static void display_input_set_lock(bool is_locked) {
	if(is_locked) {
		rtos_mutex_lock(display_input.mutex);
	}
	else {
		rtos_mutex_unlock(display_input.mutex);
	}
}
