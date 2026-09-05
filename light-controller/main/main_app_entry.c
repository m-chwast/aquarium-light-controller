#include "main_app_entry.h"

#include "console/console.h"
#include "display/display.h"
#include "rtos.h"

void main_app_entry(void) {
	console_init();
	display_init();

	while(1) {
		rtos_delay_ms(1000);
	}
}
