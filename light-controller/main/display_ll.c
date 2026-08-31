#include "display_ll.h"

#include "esp_lcd_ili9341.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "periphs.h"

#define TAG "LL_DISP"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define INITIAL_COLOR 0x03A0

static void* callback_ctx = NULL;

static bool lcd_callback(esp_lcd_panel_io_handle_t panel_io,
						 esp_lcd_panel_io_event_data_t* edata, void* user_ctx);

bool display_ll_init(void) {
	bool result = true;

	ESP_LOGI(TAG, "Hello, LCD!");

	ESP_LOGI(TAG, "Initialize SPI bus");
	const spi_bus_config_t bus_config =
		ILI9341_PANEL_BUS_SPI_CONFIG(GPIO_LCD_PCLK, GPIO_LCD_MOSI,
									 LCD_HEIGHT * 80 * sizeof(uint16_t));
	ESP_ERROR_CHECK(
		spi_bus_initialize(SPI_HOST_LCD, &bus_config, SPI_DMA_CH_AUTO));

	ESP_LOGI(TAG, "Install panel IO");
	esp_lcd_panel_io_handle_t io_handle = NULL;
	const esp_lcd_panel_io_spi_config_t io_config = ILI9341_PANEL_IO_SPI_CONFIG(
		GPIO_LCD_CS, GPIO_LCD_DC, lcd_callback, &callback_ctx);
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
		(esp_lcd_spi_bus_handle_t)SPI_HOST_LCD, &io_config, &io_handle));

	/**
	 * Uncomment these lines if use custom initialization commands.
	 * The array should be declared as "static const" and positioned outside the
	 * function.
	 */
	// static const ili9341_lcd_init_cmd_t lcd_init_cmds[] = {
	// // {cmd, { data }, data_size, delay_ms}
	//    {0xCF, (uint8_t []){0x00, 0xAA, 0XE0}, 3, 0},
	//    {0xED, (uint8_t []){0x67, 0x03, 0X12, 0X81}, 4, 0},
	//    {0xE8, (uint8_t []){0x8A, 0x01, 0x78}, 3, 0},
	//     ...
	// };

	ESP_LOGI(TAG, "Install ILI9341 panel driver");
	esp_lcd_panel_handle_t panel_handle = NULL;
	// ili9341_vendor_config_t vendor_config = {  // Uncomment these lines if
	// use custom initialization commands
	//     .init_cmds = lcd_init_cmds,
	//     .init_cmds_size = sizeof(lcd_init_cmds) /
	//     sizeof(ili9341_lcd_init_cmd_t),
	// };
	const esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = GPIO_LCD_RST,				 // Set to -1 if not use
		.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,	 // RGB element order: R-G-B
		.bits_per_pixel = 16,  // Implemented by LCD command `3Ah` (16/18)
		// .vendor_config = &vendor_config,          // Uncomment this line if
		// use custom initialization commands
	};
	ESP_ERROR_CHECK(
		esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

	// RGB565 green = 0x07E0
	uint16_t* buffer = malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));

	if(buffer == NULL) {
		ESP_LOGE(TAG, "Failed to allocate memory for LCD buffer");
		result = false;
	}
	else {
		for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
			buffer[i] = INITIAL_COLOR;
		}

		ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH,
												  LCD_HEIGHT, buffer));

		free(buffer);
	}

	ESP_LOGI(TAG, "LCD task initialization complete");

	return result;
}

static bool lcd_callback(esp_lcd_panel_io_handle_t panel_io,
						 esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
	return true;
}
