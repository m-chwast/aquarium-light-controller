#include "display_calibration.h"

#include "display_request.h"
#include "esp_log.h"
#include "rtos.h"
#include "settings.h"

#define TAG "DISP_CALIB"

#define DISPLAY_CALIBRATION_POINT_COUNT 5

typedef struct display_calibration_t {
	const display_calibration_point_t
		target_points[DISPLAY_CALIBRATION_POINT_COUNT];
	display_calibration_point_t raw_points[DISPLAY_CALIBRATION_POINT_COUNT];

	bool is_initialized;

	rtos_task_t task;
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

void display_calibration_init(void) {
	ESP_LOGI(TAG, "Initializing");
	display_calibration.task =
		rtos_create_task(display_calibration_handler, TAG,
						 RTOS_TASK_STACK_SIZE_2KB, RTOS_PRIORITY_LOW);
}

void display_calibration_start(void) {}

void display_calibration_abort(void) {}

void display_calibration_reset(void) {}

bool display_calibration_is_active(void) { return false; }

display_calibration_point_t display_calibration_get_calibrated_coordinates(
	display_calibration_point_t raw_point) {
	return raw_point;
}

static void display_calibration_handler(void* arg) {
	const bool is_calib_ever_done =
		settings_get_bool(SETTINGS_ELEM_DISPLAY_CALIB_IS_PERFORMED);

	if(is_calib_ever_done) {
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
		display_calibration.is_initialized = true;
		ESP_LOGI(TAG, "Calibration data loaded from settings");
	}
	else {
		ESP_LOGI(
			TAG,
			"No calibration data found in settings, requesting calibration");
		display_request_send(DISPLAY_REQUEST_TYPE_CALIBRATION);
	}

	while(1) {
		rtos_delay_ms(1000);
		display_calibration_manage();
	};
}

static void display_calibration_manage(void) {}
