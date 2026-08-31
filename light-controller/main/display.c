#include "display.h"

#include "display_ll.h"
#include "display_lvgl.h"

void display_init(void) {
	display_ll_init();
	display_lvgl_init();
}
