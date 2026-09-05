#pragma once

typedef struct rtos_task_t {
	void* handle;
} rtos_task_t;

typedef void (*rtos_task_handler_t)(void*);

typedef enum rtos_task_priority_t {
	RTOS_PRIORITY_IDLE = 0,
	RTOS_PRIORITY_LOW = 1,
	RTOS_PRIORITY_NORMAL_LOW = 2,
	RTOS_PRIORITY_NORMAL = 3,
	RTOS_PRIORITY_NORMAL_HIGH = 4,
	RTOS_PRIORITY_HIGH = 5,
} rtos_task_priority_t;

void rtos_delay_ms(unsigned ms);

rtos_task_t rtos_create_task(rtos_task_handler_t task_function,
							 const char* name, unsigned stack_size_bytes,
							 rtos_task_priority_t priority);
