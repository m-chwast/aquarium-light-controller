#pragma once

#include <stdbool.h>

void display_calibration_start(void);

void display_calibration_abort(void);

void display_calibration_reset(void);

bool display_calibration_is_active(void);

void display_calibration_manage(void);
