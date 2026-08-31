#include "rtos.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void rtos_delay_ms(unsigned ms) {
	const unsigned ticks = pdMS_TO_TICKS(ms);
	vTaskDelay(ticks);
}
