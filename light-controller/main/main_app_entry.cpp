#include "main_app_entry.h"

#include "display.hpp"
#include "rtos.hpp"

static RTOS rtos{};

static Display display{rtos};

void main_app_entry(void) {
	display.init();

	while(1) {
		rtos.delay_ms(1000);
	}
}
