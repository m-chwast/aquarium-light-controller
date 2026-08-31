#pragma once

#include <stdbool.h>

bool display_ll_init(void);

unsigned display_ll_get_width(void);
unsigned display_ll_get_height(void);

void* display_ll_get_panel_handle(void);
void* display_ll_get_io_handle(void);
