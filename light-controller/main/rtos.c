#include "rtos.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void rtos_fault_handler(void);

void rtos_delay_ms(unsigned ms) {
	const unsigned ticks = pdMS_TO_TICKS(ms);
	vTaskDelay(ticks);
}

rtos_task_t rtos_create_task(rtos_task_handler_t task_function,
							 const char* name, unsigned stack_size_bytes,
							 rtos_task_priority_t priority) {
	rtos_task_t task = {0};

	uint32_t stack_size = stack_size_bytes / sizeof(StackType_t);

	if(stack_size * sizeof(StackType_t) < stack_size_bytes) {
		stack_size++;
	}

	TaskHandle_t handle = NULL;
	xTaskCreate(task_function, name, stack_size, NULL, priority, &handle);

	task.handle = handle;

	if(task.handle == NULL) {
		rtos_fault_handler();
	}

	return task;
}

static void rtos_fault_handler(void) {
	while(1) {
	};
}
