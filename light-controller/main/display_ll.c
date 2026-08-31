#include "display_ll.h"

#include "esp_lcd_ili9341.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "periphs.h"

#define TAG "LL_DISP"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

static void* callback_ctx = NULL;

static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

static bool lcd_callback(esp_lcd_panel_io_handle_t panel_io,
						 esp_lcd_panel_io_event_data_t* edata, void* user_ctx);

bool display_ll_init(void) {
	bool result = true;

	ESP_LOGI(TAG, "Hello, LCD!");

	ESP_LOGI(TAG, "Initialize SPI bus");
	const spi_bus_config_t bus_config = ILI9341_PANEL_BUS_SPI_CONFIG(
		GPIO_LCD_PCLK, GPIO_LCD_MOSI, LCD_HEIGHT * 80 * sizeof(uint16_t));
	ESP_ERROR_CHECK(
		spi_bus_initialize(SPI_HOST_LCD, &bus_config, SPI_DMA_CH_AUTO));

	ESP_LOGI(TAG, "Install panel IO");

	const esp_lcd_panel_io_spi_config_t io_config = ILI9341_PANEL_IO_SPI_CONFIG(
		GPIO_LCD_CS, GPIO_LCD_DC, lcd_callback, &callback_ctx);
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
		(esp_lcd_spi_bus_handle_t)SPI_HOST_LCD, &io_config, &io_handle));

	ESP_LOGI(TAG, "Install ILI9341 panel driver");
	const esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = GPIO_LCD_RST,				 // Set to -1 if not use
		.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,	 // RGB element order: R-G-B
		.bits_per_pixel = 16,  // Implemented by LCD command `3Ah` (16/18)
	};
	ESP_ERROR_CHECK(
		esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

	if(result) {
		ESP_LOGI(TAG, "LCD task initialization complete");
	}
	else {
		ESP_LOGE(TAG, "LCD task initialization failed");
	}

	return result;
}

unsigned display_ll_get_width(void) { return LCD_WIDTH; }

unsigned display_ll_get_height(void) { return LCD_HEIGHT; }

void* display_ll_get_panel_handle(void) { return panel_handle; }

void* display_ll_get_io_handle(void) { return io_handle; }

static bool lcd_callback(esp_lcd_panel_io_handle_t panel_io,
						 esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
	return true;
}
