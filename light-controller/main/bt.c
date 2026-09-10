#include "bt.h"

#include <stdio.h>

#include "bt_request.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "host/ble_gap.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "rtos.h"

#define TAG "BLE"

typedef struct bt_t {
	rtos_task_t task;
} bt_t;

static bt_t bt;

static void on_sync(void);
static void nimble_host_task(void* param);

static void bt_task(void* param);

void bt_init(void) {
	// bt esp stack init

	ESP_ERROR_CHECK(nimble_port_init());

	ble_hs_cfg.sync_cb = on_sync;

	nimble_port_freertos_init(nimble_host_task);

	// app code init

	bt_request_init();

	bt.task = rtos_create_task(bt_task, TAG, RTOS_TASK_STACK_SIZE_2KB,
							   RTOS_PRIORITY_LOW);
}

static int gap_event(struct ble_gap_event* event, void* arg) {
	switch(event->type) {
		case BLE_GAP_EVENT_DISC: {
			const struct ble_gap_disc_desc* d = &event->disc;

			struct ble_hs_adv_fields fields;
			int rc = ble_hs_adv_parse_fields(&fields, d->data, d->length_data);

			if(rc != 0) {
				ESP_LOGW(TAG, "Failed to parse advertising data: %d", rc);
				return 0;
			}

			char addr_str[18];

			snprintf(addr_str, sizeof(addr_str),
					 "%02X:%02X:%02X:%02X:%02X:%02X", d->addr.val[5],
					 d->addr.val[4], d->addr.val[3], d->addr.val[2],
					 d->addr.val[1], d->addr.val[0]);

			if(fields.name != NULL && fields.name_len > 0) {
				ESP_LOGI(TAG, "%s  RSSI: %d  Name: %.*s", addr_str, d->rssi,
						 fields.name_len, fields.name);
			}
			else {
				ESP_LOGI(TAG, "%s  RSSI: %d  Name: <unknown>", addr_str,
						 d->rssi);
			}

			return 0;
		}

		case BLE_GAP_EVENT_DISC_COMPLETE:
			ESP_LOGI(TAG, "Scan complete");
			return 0;

		default:
			return 0;
	}
}

static void start_scan(void) {
	struct ble_gap_disc_params params = {
		.itvl = 0,
		.window = 0,
		.filter_policy = BLE_HCI_SCAN_FILT_NO_WL,
		.passive = 1,
		.limited = 0,
		.filter_duplicates = 1,
	};

	int rc = ble_gap_disc(BLE_OWN_ADDR_PUBLIC,
						  10000,  // 10 seconds
						  &params, gap_event, NULL);

	if(rc != 0) {
		ESP_LOGE(TAG, "ble_gap_disc failed: %d", rc);
	}
}

static void on_sync(void) { start_scan(); }

static void nimble_host_task(void* param) {
	nimble_port_run();
	nimble_port_freertos_deinit();
}

static void bt_task(void* param) {
	bt_request_type_t request_type = BT_REQUEST_TYPE_NONE;

	while(1) {
		switch(request_type) {
			case BT_REQUEST_TYPE_START_SCAN:
				start_scan();
				break;
			case BT_REQUEST_TYPE_NONE:
				break;
			default:
				break;
		}

		if(request_type == BT_REQUEST_TYPE_NONE) {
			rtos_delay_ms(100);
		}

		request_type = bt_request_receive();
	}
}