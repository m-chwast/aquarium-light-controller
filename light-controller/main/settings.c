#include "settings.h"

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "rtos.h"

#define TAG "SETTINGS"

#define SETTINGS_NAMESPACE "store"

typedef struct settings_storage_t {
	int display_calib_x1;
	int display_calib_y1;
	int display_calib_x2;
	int display_calib_y2;
	int display_calib_x3;
	int display_calib_y3;
	int display_calib_x4;
	int display_calib_y4;
	int display_calib_x5;
	int display_calib_y5;

	bool display_calib_is_performed;
} settings_storage_t;

typedef struct settings_t {
	rtos_task_t task;
	bool save_request;
} settings_t;

static settings_storage_t settings_storage;

static settings_t settings;

static const char* settings_get_key(settings_elem_t elem);
static int* settings_get_int_ptr(settings_elem_t elem);
static bool* settings_get_bool_ptr(settings_elem_t elem);

static void settings_task_handler(void* arg);

static void settings_read_all(void);

void settings_init(void) {
	esp_err_t err = nvs_flash_init();
	if(err == ESP_ERR_NVS_NO_FREE_PAGES ||
	   err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);

	settings_read_all();

	settings.save_request = false;
	settings.task =
		rtos_create_task(settings_task_handler, TAG, RTOS_TASK_STACK_SIZE_4KB,
						 RTOS_PRIORITY_LOW);
}

int settings_get_int(settings_elem_t elem) {
	int value = 0;

	const int* const value_ptr = settings_get_int_ptr(elem);
	if(value_ptr != NULL) {
		value = *value_ptr;
	}

	return value;
}

void settings_set_int(settings_elem_t elem, int value) {
	int* const value_ptr = settings_get_int_ptr(elem);
	if(value_ptr != NULL) {
		if(value != *value_ptr) {
			const char* const key = settings_get_key(elem);

			ESP_LOGI(TAG, "Setting %s to %d", key, value);
			*value_ptr = value;

			settings.save_request = true;
		}
	}
}

bool settings_get_bool(settings_elem_t elem) {
	bool value = false;

	bool* const value_ptr = settings_get_bool_ptr(elem);
	if(value_ptr != NULL) {
		value = *value_ptr;
	}

	return value;
}

void settings_set_bool(settings_elem_t elem, bool value) {
	bool* const value_ptr = settings_get_bool_ptr(elem);
	if(value_ptr != NULL) {
		if(value != *value_ptr) {
			const char* const key = settings_get_key(elem);

			ESP_LOGI(TAG, "Setting %s to %d", key, value);
			*value_ptr = value;

			settings.save_request = true;
		}
	}
}

static int* settings_get_int_ptr(settings_elem_t elem) {
	int* value_ptr = NULL;

	switch(elem) {
		case SETTINGS_ELEM_DISPLAY_CALIB_X1:
			value_ptr = &settings_storage.display_calib_x1;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y1:
			value_ptr = &settings_storage.display_calib_y1;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X2:
			value_ptr = &settings_storage.display_calib_x2;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y2:
			value_ptr = &settings_storage.display_calib_y2;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X3:
			value_ptr = &settings_storage.display_calib_x3;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y3:
			value_ptr = &settings_storage.display_calib_y3;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X4:
			value_ptr = &settings_storage.display_calib_x4;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y4:
			value_ptr = &settings_storage.display_calib_y4;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X5:
			value_ptr = &settings_storage.display_calib_x5;
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y5:
			value_ptr = &settings_storage.display_calib_y5;
			break;
		default: {
			const char* const elem_name = settings_get_key(elem);
			ESP_LOGE(TAG, "Invalid settings element: %d, %s", elem, elem_name);
			break;
		}
	}

	return value_ptr;
}

static bool* settings_get_bool_ptr(settings_elem_t elem) {
	bool* value_ptr = NULL;

	switch(elem) {
		case SETTINGS_ELEM_DISPLAY_CALIB_IS_PERFORMED:
			value_ptr = (bool*)&settings_storage.display_calib_is_performed;
			break;
		default: {
			const char* const elem_name = settings_get_key(elem);
			ESP_LOGE(TAG, "Invalid settings element: %d, %s", elem, elem_name);
			break;
		}
	}

	return value_ptr;
}

static const char* settings_get_key(settings_elem_t elem) {
	const char* key = "";

	switch(elem) {
		case SETTINGS_ELEM_DISPLAY_CALIB_X1:
			key = "disp_calib_x1";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y1:
			key = "disp_calib_y1";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X2:
			key = "disp_calib_x2";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y2:
			key = "disp_calib_y2";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X3:
			key = "disp_calib_x3";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y3:
			key = "disp_calib_y3";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X4:
			key = "disp_calib_x4";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y4:
			key = "disp_calib_y4";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_X5:
			key = "disp_calib_x5";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_Y5:
			key = "disp_calib_y5";
			break;
		case SETTINGS_ELEM_DISPLAY_CALIB_IS_PERFORMED:
			key = "disp_calib_done";
			break;
		default:
			key = "unknown";
			ESP_LOGE(TAG, "Invalid settings element: %d", elem);
			break;
	}

	const int key_length = strlen(key);
	if(key_length > 15) {
		ESP_LOGE(TAG, "Key length exceeds maximum allowed length: %d",
				 key_length);
		key = "";
	}

	return key;
}

static void settings_task_handler(void* arg) {
	ESP_LOGI(TAG, "Settings task started");

	while(true) {
		if(settings.save_request) {
			ESP_LOGD(TAG, "Saving settings to NVS");

			nvs_handle_t nvs_handle = {0};
			const esp_err_t err =
				nvs_open(SETTINGS_NAMESPACE, NVS_READWRITE, &nvs_handle);
			if(err != ESP_OK) {
				const char* const err_name = esp_err_to_name(err);
				ESP_LOGE(TAG, "Error opening NVS handle: %s", err_name);
			}
			else {
				bool nvs_commit_needed = false;

				for(int elem = SETTINGS_ELEM_TYPEBEGIN_INT + 1;
					elem < SETTINGS_ELEM_TYPEEND_INT; elem++) {
					const char* const key = settings_get_key(elem);
					const int volatile_value = settings_get_int(elem);

					int32_t out_value = 0;
					const esp_err_t err =
						nvs_get_i32(nvs_handle, key, &out_value);
					const int stored_value = out_value;

					if((err == ESP_OK) || (err == ESP_ERR_NVS_NOT_FOUND) ||
					   (stored_value != volatile_value)) {
						nvs_set_i32(nvs_handle, key, volatile_value);
						nvs_commit_needed = true;
						ESP_LOGI(TAG, "Saved setting %s = %d", key,
								 volatile_value);
					}
					else {
						const char* const err_name = esp_err_to_name(err);
						ESP_LOGE(TAG, "Error reading setting %s from NVS: %s",
								 key, err_name);
					}
				}

				for(int elem = SETTINGS_ELEM_TYPEBEGIN_BOOL + 1;
					elem < SETTINGS_ELEM_TYPEEND_BOOL; elem++) {
					const char* const key = settings_get_key(elem);
					const bool volatile_value = *settings_get_bool_ptr(elem);

					uint8_t out_value = 0;
					const esp_err_t err =
						nvs_get_u8(nvs_handle, key, &out_value);
					const bool stored_value = (out_value != 0);

					if((err == ESP_OK) || (err == ESP_ERR_NVS_NOT_FOUND) ||
					   (stored_value != volatile_value)) {
						nvs_set_u8(nvs_handle, key, volatile_value ? 1 : 0);
						nvs_commit_needed = true;
						ESP_LOGI(TAG, "Saved setting %s = %d", key,
								 volatile_value ? 1 : 0);
					}
					else {
						const char* const err_name = esp_err_to_name(err);
						ESP_LOGE(TAG, "Error reading setting %s from NVS: %s",
								 key, err_name);
					}
				}

				if(nvs_commit_needed) {
					ESP_LOGD(TAG, "Committing changes to NVS");
					const esp_err_t commit_err = nvs_commit(nvs_handle);
					const char* const commit_err_name =
						esp_err_to_name(commit_err);
					if(commit_err != ESP_OK) {
						ESP_LOGE(TAG, "Error committing changes to NVS: %s",
								 commit_err_name);
					}
				}
			}

			nvs_close(nvs_handle);

			settings.save_request = false;
		}

		rtos_delay_ms(1000);
	}
}

static void settings_read_all(void) {
	nvs_handle_t nvs_handle = {0};
	const esp_err_t err =
		nvs_open(SETTINGS_NAMESPACE, NVS_READWRITE, &nvs_handle);
	if(err != ESP_OK) {
		const char* const err_name = esp_err_to_name(err);
		ESP_LOGE(TAG, "Error opening NVS handle: %s", err_name);
	}
	else {
		for(int elem = SETTINGS_ELEM_TYPEBEGIN_INT + 1;
			elem < SETTINGS_ELEM_TYPEEND_INT; elem++) {
			const char* const key = settings_get_key(elem);
			int32_t out_value = 0;
			const esp_err_t err = nvs_get_i32(nvs_handle, key, &out_value);
			if(err == ESP_OK) {
				int* const value_ptr = settings_get_int_ptr(elem);
				if(value_ptr != NULL) {
					*value_ptr = out_value;
					ESP_LOGI(TAG, "Read setting %s = %d", key, out_value);
				}
			}
			else if(err == ESP_ERR_NVS_NOT_FOUND) {
				ESP_LOGI(TAG, "Setting %s not found in NVS", key);
			}
			else {
				const char* const err_name = esp_err_to_name(err);
				ESP_LOGE(TAG, "Error reading setting %s from NVS: %s", key,
						 err_name);
			}
		}

		for(int elem = SETTINGS_ELEM_TYPEBEGIN_BOOL + 1;
			elem < SETTINGS_ELEM_TYPEEND_BOOL; elem++) {
			const char* const key = settings_get_key(elem);
			uint8_t out_value = 0;
			const esp_err_t err = nvs_get_u8(nvs_handle, key, &out_value);
			if(err == ESP_OK) {
				bool* const value_ptr = settings_get_bool_ptr(elem);
				if(value_ptr != NULL) {
					*value_ptr = (out_value != 0);
					ESP_LOGI(TAG, "Read setting %s = %d", key, out_value);
				}
			}
			else if(err == ESP_ERR_NVS_NOT_FOUND) {
				ESP_LOGI(TAG, "Setting %s not found in NVS", key);
			}
			else {
				const char* const err_name = esp_err_to_name(err);
				ESP_LOGE(TAG, "Error reading setting %s from NVS: %s", key,
						 err_name);
			}
		}

		nvs_close(nvs_handle);
	}
}
