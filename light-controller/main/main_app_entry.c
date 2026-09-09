#include "main_app_entry.h"

#include "bt.h"
#include "console/console.h"
#include "display/display.h"
#include "rtos.h"
#include "settings.h"

void main_app_entry(void) {
	console_init();

	settings_init();

	display_init();

	bt_init();

	while(1) {
		rtos_delay_ms(1000);
	}
}
