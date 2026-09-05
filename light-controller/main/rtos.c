#include "rtos.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

static void rtos_fault_handler(void);

void rtos_delay_ms(unsigned ms) {
	const unsigned ticks = pdMS_TO_TICKS(ms);
	vTaskDelay(ticks);
}

rtos_task_t rtos_create_task(rtos_task_handler_t task_function,
							 const char* name,
							 rtos_task_stack_size_t stack_size,
							 rtos_task_priority_t priority) {
	rtos_task_t task = {0};

	uint32_t stack_size_words = stack_size / sizeof(StackType_t);

	if(stack_size_words * sizeof(StackType_t) < stack_size) {
		stack_size_words++;
	}

	TaskHandle_t handle = NULL;
	xTaskCreate(task_function, name, stack_size_words, NULL, priority, &handle);

	task.handle = handle;

	if(task.handle == NULL) {
		rtos_fault_handler();
	}

	return task;
}

rtos_queue_t rtos_create_queue(unsigned item_count, unsigned item_size) {
	rtos_queue_t queue = {0};

	QueueHandle_t handle = xQueueCreate(item_count, item_size);
	if(handle == NULL) {
		rtos_fault_handler();
	}

	queue.handle = handle;
	return queue;
}

void rtos_queue_send(const rtos_queue_t queue, const void* item) {
	const QueueHandle_t handle = (QueueHandle_t)queue.handle;
	if(xQueueSend(handle, item, portMAX_DELAY) != pdTRUE) {
		rtos_fault_handler();
	}
}

void rtos_queue_receive(const rtos_queue_t queue, void* item) {
	const QueueHandle_t handle = (QueueHandle_t)queue.handle;
	if(xQueueReceive(handle, item, portMAX_DELAY) != pdTRUE) {
		rtos_fault_handler();
	}
}

bool rtos_queue_is_empty(const rtos_queue_t queue) {
	const QueueHandle_t handle = (QueueHandle_t)queue.handle;
	const BaseType_t message_count = uxQueueMessagesWaiting(handle);

	if(message_count == 0) {
		return true;
	}
	return false;
}

unsigned rtos_queue_get_free_space(const rtos_queue_t queue) {
	const QueueHandle_t handle = (QueueHandle_t)queue.handle;
	const BaseType_t space_available = uxQueueSpacesAvailable(handle);
	const unsigned space_u = (unsigned)space_available;
	return space_u;
}

void rtos_queue_reset(const rtos_queue_t queue) {
	const QueueHandle_t handle = (QueueHandle_t)queue.handle;
	const BaseType_t result = xQueueReset(handle);

	if(result != pdTRUE) {
		rtos_fault_handler();
	}
}

static void rtos_fault_handler(void) {
	while(1) {
	};
}
