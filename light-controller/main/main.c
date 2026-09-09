#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main_app_entry.h"
#include "periphs.h"
#include "esp_heap_caps.h"

#define TAG "main"

static void blink_task(void* arg);

static void stats_task(void* arg);
static void print_stats(void);

void app_main(void) {
	xTaskCreate(&blink_task, "BLINK", 1024, NULL, 5, NULL);

	xTaskCreate(&stats_task, "STATS", 4096, NULL, 5, NULL);

	main_app_entry();

	while(1) {
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static void blink_task(void* arg) {
	gpio_set_direction(GPIO_BLINK, GPIO_MODE_OUTPUT);

	while(1) {
		gpio_set_level(GPIO_BLINK, 1);
		vTaskDelay(pdMS_TO_TICKS(1000));

		gpio_set_level(GPIO_BLINK, 0);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static void stats_task(void* arg) {
	while(1) {
		vTaskDelay(pdMS_TO_TICKS(5000));

		char stats[1024] = {0};
		vTaskGetRunTimeStats(stats);

		ESP_LOGI(TAG, "Task stats:\n%s", stats);

		print_stats();
	}
}

static void print_stats(void) {
// =========================================================================
    // 1. ESP-IDF HEAP METRICS (Replacing vPortGetHeapStats)
    // =========================================================================
    multi_heap_info_t xHeapInfo;
    
    // MALLOC_CAP_8BIT fetches stats for all standard data RAM (DRAM) regions
    heap_caps_get_info(&xHeapInfo, MALLOC_CAP_8BIT);

    printf("\r\n==================================================\r\n");
    printf("                  HEAP STATISTICS                 \r\n");
    printf("==================================================\r\n");
    printf("Total Free Heap Space      : %zu bytes\r\n", xHeapInfo.total_free_bytes);
    printf("Largest Free Block Size    : %zu bytes\r\n", xHeapInfo.largest_free_block);
    printf("Minimum Free Heap Ever     : %zu bytes\r\n", heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT));
    printf("Allocated Blocks Count     : %zu\r\n", xHeapInfo.allocated_blocks);
    printf("Free Blocks Count          : %zu\r\n", xHeapInfo.free_blocks);
    printf("Total Heap Size            : %zu bytes\r\n", xHeapInfo.total_allocated_bytes + xHeapInfo.total_free_bytes);

	// =========================================================================
	// 2. TASK RAM USAGE (Using uxTaskGetSystemState)
	// =========================================================================
	// Get the total number of tasks running in the system
	UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();

	// Allocate dynamic memory to hold array of task statuses
	TaskStatus_t* pxTaskStatusArray =
		pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

	if(pxTaskStatusArray != NULL) {
		// Populate the array with task info
		uxArraySize =
			uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

		printf("\r\n==================================================\r\n");
		printf("                  TASK STATISTICS                 \r\n");
		printf("==================================================\r\n");
		printf("%-16s %-10s %-15s\r\n", "Task Name", "Priority",
			   "Min Free Stack");
		printf("--------------------------------------------------\r\n");

		for(UBaseType_t x = 0; x < uxArraySize; x++) {
			// Note: "Min Free Stack" represents the "High Water Mark".
			// It is the closest the task stack came to overflowing, measured in
			// Stack Words. On a 32-bit architecture (like ARM Cortex-M),
			// multiply this value by 4 to get bytes.
			unsigned minFreeBytes =
				(unsigned)pxTaskStatusArray[x].usStackHighWaterMark *
				sizeof(StackType_t);

			printf("%-16s %-10d %-10u bytes\r\n",
				   pxTaskStatusArray[x].pcTaskName,
				   (int)pxTaskStatusArray[x].uxCurrentPriority, minFreeBytes);
		}

		// Always free the temporary array buffer
		vPortFree(pxTaskStatusArray);
	}
	else {
		printf(
			"\r\n[Error]: Not enough heap memory to allocate TaskStatus_t "
			"array!\r\n");
	}
	printf("==================================================\r\n\r\n");
}
