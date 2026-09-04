#include "display_ll.h"

#include "esp_lcd_ili9341.h"
#include "esp_lcd_ili9341_init_cmds_2.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_xpt2046.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "periphs.h"

#define TAG "LL_DISP"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;

static void display_ll_touch_init(void);

bool display_ll_init(void) {
	bool result = true;

	ESP_LOGI(TAG, "Hello, LCD!");

	ESP_LOGI(TAG, "Initialize SPI bus");
	const int max_transfer_size = LCD_WIDTH * 40 * sizeof(uint16_t);
	const spi_bus_config_t bus_config = {.miso_io_num = GPIO_LCD_MISO,
										 .mosi_io_num = GPIO_LCD_MOSI,
										 .sclk_io_num = GPIO_LCD_PCLK,
										 .quadwp_io_num = -1,
										 .quadhd_io_num = -1,
										 .max_transfer_sz = max_transfer_size};
	ESP_ERROR_CHECK(
		spi_bus_initialize(SPI_HOST_LCD, &bus_config, SPI_DMA_CH_AUTO));

	ESP_LOGI(TAG, "Install panel IO");

	const esp_lcd_panel_io_spi_config_t io_config =
		ILI9341_PANEL_IO_SPI_CONFIG(GPIO_LCD_CS, GPIO_LCD_DC, NULL, NULL);
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
		(esp_lcd_spi_bus_handle_t)SPI_HOST_LCD, &io_config, &io_handle));

	ESP_LOGI(TAG, "Install ILI9341 panel driver");
	ili9341_vendor_config_t vendor_config = {
		.init_cmds = ili9341_lcd_init_vendor,
		.init_cmds_size =
			sizeof(ili9341_lcd_init_vendor) / sizeof(ili9341_lcd_init_cmd_t),
	};
	const esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = GPIO_LCD_RST,				 // Set to -1 if not use
		.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,	 // RGB element order: RGB
		.bits_per_pixel = 16,  // Implemented by LCD command `3Ah` (16/18)
		.vendor_config = &vendor_config,
	};
	ESP_ERROR_CHECK(
		esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

	display_ll_touch_init();

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

static void display_ll_touch_init(void) {
	ESP_LOGI(TAG, "Initialize touch controller");

	esp_lcd_touch_handle_t tp = NULL;
	esp_lcd_panel_io_handle_t tp_io_handle = NULL;
	esp_lcd_panel_io_spi_config_t tp_io_config =
		ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(GPIO_TOUCH_CS);
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
		(esp_lcd_spi_bus_handle_t)SPI_HOST_LCD, &tp_io_config, &tp_io_handle));

	const unsigned lcd_width = display_ll_get_width();
	const unsigned lcd_height = display_ll_get_height();

	esp_lcd_touch_config_t tp_cfg = {
		.x_max = lcd_width,
		.y_max = lcd_height,
		.rst_gpio_num = -1,
		.int_gpio_num = -1,
		.flags =
			{
				.swap_xy = 0,
				.mirror_x = 0,
				.mirror_y = 0,
			},
	};

	ESP_LOGI(TAG, "Initialize touch controller XPT2046");
	ESP_ERROR_CHECK(esp_lcd_touch_new_spi_xpt2046(tp_io_handle, &tp_cfg, &tp));
	ESP_LOGI(TAG, "Touch controller initialized");
}