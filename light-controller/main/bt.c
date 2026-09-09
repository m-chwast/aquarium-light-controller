#include "bt.h"

#include <stdio.h>

#include "esp_log.h"
#include "host/ble_gap.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"

static const char* TAG = "BLE_SCAN";

static void on_sync(void);
static void nimble_host_task(void* param);

void bt_init(void) {
	ESP_ERROR_CHECK(nimble_port_init());

	ble_hs_cfg.sync_cb = on_sync;

	nimble_port_freertos_init(nimble_host_task);
}

static int gap_event(struct ble_gap_event* event, void* arg) {
	switch(event->type) {
		case BLE_GAP_EVENT_DISC: {
			const struct ble_gap_disc_desc* d = &event->disc;

			char addr_str[18];

			snprintf(addr_str, sizeof(addr_str),
					 "%02X:%02X:%02X:%02X:%02X:%02X", d->addr.val[5],
					 d->addr.val[4], d->addr.val[3], d->addr.val[2],
					 d->addr.val[1], d->addr.val[0]);

			ESP_LOGI(TAG, "Device: %s RSSI: %d", addr_str, d->rssi);

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
