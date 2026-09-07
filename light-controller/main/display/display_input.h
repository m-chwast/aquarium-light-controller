#pragma once

#include <stdbool.h>

typedef struct display_input_data_t {
	int x;
	int y;
	bool is_pressed;
} display_input_data_t;

void display_input_init(void);

void display_input_provide_data(display_input_data_t data);

display_input_data_t display_input_get_data(void);

// data before calibration
display_input_data_t display_input_get_data_raw(void);
