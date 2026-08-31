#include "main_app_entry.h"

#include "display.h"
#include "rtos.h"

void main_app_entry(void) {
	display_init();

	while(1) {
		rtos_delay_ms(1000);
	}
}
