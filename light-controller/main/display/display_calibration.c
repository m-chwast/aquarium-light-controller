#include "display_calibration.h"

#include "display_input.h"
#include "display_request.h"
#include "esp_log.h"
#include "rtos.h"
#include "settings.h"

#define TAG "DISP_CALIB"

#define DISPLAY_CALIBRATION_POINT_COUNT 5

typedef enum display_calibration_state_t {
	DISPLAY_CALIBRATION_STATE_IDLE,
	DISPLAY_CALIBRATION_STATE_STARTED,
	DISPLAY_CALIBRATION_STATE_P1_AWAIT,
	DISPLAY_CALIBRATION_STATE_P1,
	DISPLAY_CALIBRATION_STATE_P2_AWAIT,
	DISPLAY_CALIBRATION_STATE_P2,
	DISPLAY_CALIBRATION_STATE_P3_AWAIT,
	DISPLAY_CALIBRATION_STATE_P3,
	DISPLAY_CALIBRATION_STATE_P4_AWAIT,
	DISPLAY_CALIBRATION_STATE_P4,
	DISPLAY_CALIBRATION_STATE_P5_AWAIT,
	DISPLAY_CALIBRATION_STATE_P5,
	DISPLAY_CALIBRATION_STATE_FINISHED,
} display_calibration_state_t;

typedef struct display_calibration_t {
	const display_calibration_point_t
		target_points[DISPLAY_CALIBRATION_POINT_COUNT];
	display_calibration_point_t raw_points[DISPLAY_CALIBRATION_POINT_COUNT];

	bool is_initialized;

	rtos_task_t task;

	display_calibration_state_t state;
	int current_point_index;
} display_calibration_t;

static display_calibration_t display_calibration = {
	.target_points =
		{
			{20, 20},	 // Top-left
			{300, 20},	 // Top-right
			{300, 220},	 // Bottom-right
			{20, 220},	 // Bottom-left
			{160, 120}	 // Center
		},
};

static void display_calibration_handler(void* arg);
static void display_calibration_manage(void);
static void display_calibration_load_all(void);
static bool display_calibration_check_input(int* x, int* y);

void display_calibration_init(void) {
	ESP_LOGI(TAG, "Initializing");
	display_calibration.task =
		rtos_create_task(display_calibration_handler, TAG,
						 RTOS_TASK_STACK_SIZE_2KB, RTOS_PRIORITY_LOW);
}

void display_calibration_start(void) {
	display_calibration.state = DISPLAY_CALIBRATION_STATE_STARTED;
	display_calibration.current_point_index = 0;
}

void display_calibration_abort(void) {
	display_calibration.state = DISPLAY_CALIBRATION_STATE_IDLE;
	display_calibration.current_point_index = 0;
	display_calibration_load_all();
}

bool display_calibration_is_active(void) {
	const bool is_active =
		(display_calibration.state != DISPLAY_CALIBRATION_STATE_IDLE);
	return is_active;
}

display_calibration_point_t display_calibration_get_calibrated_coordinates(
	display_calibration_point_t raw_point) {
	return raw_point;
}

display_calibration_point_t display_calibration_get_current_target_point(void) {
	const int index = display_calibration.current_point_index;

	display_calibration_point_t point = {0};

	if((index >= 0) && (index < DISPLAY_CALIBRATION_POINT_COUNT)) {
		point = display_calibration.target_points[index];
	}

	return point;
}

static void display_calibration_handler(void* arg) {
	const bool is_calib_ever_done =
		settings_get_bool(SETTINGS_ELEM_DISPLAY_CALIB_IS_PERFORMED);

	if(is_calib_ever_done) {
		display_calibration_load_all();

		display_calibration.is_initialized = true;

		ESP_LOGI(TAG, "Calibration data loaded from settings");
	}
	else {
		display_calibration.is_initialized = false;

		ESP_LOGI(TAG, "No calibration data found, requesting calibration");

		display_request_send(DISPLAY_REQUEST_TYPE_CALIBRATION);
	}

	while(1) {
		rtos_delay_ms(200);
		display_calibration_manage();
	};
}

static void display_calibration_set_state(
	display_calibration_state_t new_state) {
	display_calibration.state = new_state;
}

static void display_calibration_record_point(int index, int x, int y) {
	ESP_LOGI(TAG, "Calibration point %d recorded: (%d, %d)", index + 1, x, y);
	display_calibration.raw_points[index].x = x;
	display_calibration.raw_points[index].y = y;
}

static void display_calibration_manage(void) {
	int x = 0;
	int y = 0;

	const bool is_pressed = display_input_is_pressed();

	switch(display_calibration.state) {
		case DISPLAY_CALIBRATION_STATE_IDLE: {
			break;
		}
		case DISPLAY_CALIBRATION_STATE_STARTED: {
			display_calibration_set_state(DISPLAY_CALIBRATION_STATE_P1_AWAIT);
			display_calibration.current_point_index = 0;
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P1_AWAIT: {
			if(!is_pressed) {
				display_calibration_set_state(DISPLAY_CALIBRATION_STATE_P1);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P1: {
			const bool point_ok = display_calibration_check_input(&x, &y);

			if(point_ok) {
				display_calibration_record_point(0, x, y);
				display_calibration_set_state(
					DISPLAY_CALIBRATION_STATE_P2_AWAIT);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P2_AWAIT: {
			display_calibration.current_point_index = 1;
			if(!is_pressed) {
				display_calibration_set_state(DISPLAY_CALIBRATION_STATE_P2);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P2: {
			const bool point_ok = display_calibration_check_input(&x, &y);

			if(point_ok) {
				display_calibration_record_point(1, x, y);
				display_calibration_set_state(
					DISPLAY_CALIBRATION_STATE_P3_AWAIT);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P3_AWAIT: {
			display_calibration.current_point_index = 2;
			if(!is_pressed) {
				display_calibration_set_state(DISPLAY_CALIBRATION_STATE_P3);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P3: {
			const bool point_ok = display_calibration_check_input(&x, &y);

			if(point_ok) {
				display_calibration_record_point(2, x, y);
				display_calibration_set_state(
					DISPLAY_CALIBRATION_STATE_P4_AWAIT);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P4_AWAIT: {
			display_calibration.current_point_index = 3;
			if(!is_pressed) {
				display_calibration_set_state(DISPLAY_CALIBRATION_STATE_P4);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P4: {
			const bool point_ok = display_calibration_check_input(&x, &y);

			if(point_ok) {
				display_calibration_record_point(3, x, y);
				display_calibration_set_state(
					DISPLAY_CALIBRATION_STATE_P5_AWAIT);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P5_AWAIT: {
			display_calibration.current_point_index = 4;
			if(!is_pressed) {
				display_calibration_set_state(DISPLAY_CALIBRATION_STATE_P5);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_P5: {
			const bool point_ok = display_calibration_check_input(&x, &y);

			if(point_ok) {
				display_calibration_record_point(4, x, y);
				display_calibration_set_state(
					DISPLAY_CALIBRATION_STATE_FINISHED);
			}
			break;
		}
		case DISPLAY_CALIBRATION_STATE_FINISHED: {
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_X1,
							 display_calibration.raw_points[0].x);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_Y1,
							 display_calibration.raw_points[0].y);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_X2,
							 display_calibration.raw_points[1].x);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_Y2,
							 display_calibration.raw_points[1].y);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_X3,
							 display_calibration.raw_points[2].x);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_Y3,
							 display_calibration.raw_points[2].y);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_X4,
							 display_calibration.raw_points[3].x);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_Y4,
							 display_calibration.raw_points[3].y);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_X5,
							 display_calibration.raw_points[4].x);
			settings_set_int(SETTINGS_ELEM_DISPLAY_CALIB_Y5,
							 display_calibration.raw_points[4].y);

			settings_set_bool(SETTINGS_ELEM_DISPLAY_CALIB_IS_PERFORMED, true);

			display_calibration.is_initialized = true;
			display_calibration_load_all();

			display_calibration_set_state(DISPLAY_CALIBRATION_STATE_IDLE);

			break;
		}
	}
}

static void display_calibration_load_all(void) {
	display_calibration.raw_points[0].x =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_X1);
	display_calibration.raw_points[0].y =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_Y1);
	display_calibration.raw_points[1].x =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_X2);
	display_calibration.raw_points[1].y =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_Y2);
	display_calibration.raw_points[2].x =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_X3);
	display_calibration.raw_points[2].y =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_Y3);
	display_calibration.raw_points[3].x =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_X4);
	display_calibration.raw_points[3].y =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_Y4);
	display_calibration.raw_points[4].x =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_X5);
	display_calibration.raw_points[4].y =
		settings_get_int(SETTINGS_ELEM_DISPLAY_CALIB_Y5);
}

static bool display_calibration_check_input(int* x, int* y) {
	const display_input_data_t input_data = display_input_get_data_raw();

	bool data_stored = false;

	if(input_data.is_pressed) {
		*x = input_data.x;
		*y = input_data.y;

		data_stored = true;
	}

	return data_stored;
}
