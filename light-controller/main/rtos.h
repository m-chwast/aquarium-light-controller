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

typedef enum rtos_task_stack_size_t {
	RTOS_TASK_STACK_SIZE_1KB = 1024,
	RTOS_TASK_STACK_SIZE_2KB = 2048,
	RTOS_TASK_STACK_SIZE_4KB = 4096,
	RTOS_TASK_STACK_SIZE_8KB = 8192,
	RTOS_TASK_STACK_SIZE_16KB = 16384,
} rtos_task_stack_size_t;

void rtos_delay_ms(unsigned ms);

rtos_task_t rtos_create_task(rtos_task_handler_t task_function,
							 const char* name,
							 rtos_task_stack_size_t stack_size,
							 rtos_task_priority_t priority);
