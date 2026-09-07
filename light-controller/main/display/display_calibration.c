#include "display_calibration.h"

#include "esp_log.h"

#define TAG "DISP_CALIB"

#define DISPLAY_CALIBRATION_POINT_COUNT 5

typedef struct display_calibration_t {
	const display_calibration_point_t
		target_points[DISPLAY_CALIBRATION_POINT_COUNT];
	display_calibration_point_t raw_points[DISPLAY_CALIBRATION_POINT_COUNT];

    
    bool is_initialized;
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

void display_calibration_init(void) {
    ESP_LOGI(TAG, "Initializing");
}

void display_calibration_start(void) {}

void display_calibration_abort(void) {}

void display_calibration_reset(void) {}

bool display_calibration_is_active(void) { return false; }

void display_calibration_manage(void) {}

display_calibration_point_t display_calibration_get_calibrated_coordinates(
	display_calibration_point_t raw_point) {
	return raw_point;
}