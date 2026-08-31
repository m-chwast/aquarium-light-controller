#include "rtos.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void RTOS::delay_ms(unsigned ms) {
	const unsigned ticks = pdMS_TO_TICKS(ms);
	vTaskDelay(ticks);
}
