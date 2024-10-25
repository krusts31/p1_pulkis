#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Struct with all the pins defined
typedef struct s_pins {
	uint8_t TX;   // GPIO1
	uint8_t RX;   // GPIO3
	uint8_t D1;   // GPIO5
	uint8_t D2;   // GPIO4
	uint8_t D3;   // GPIO0
	uint8_t D4;   // GPIO2
	uint8_t GND;  // GND
	uint8_t V5;   // -
	uint8_t RST;  // RST
	uint8_t A0;   // A0
	uint8_t D0;   // GPIO16
	uint8_t D5;   // GPIO14
	uint8_t D6;   // GPIO12
	uint8_t D7;   // GPIO13
	uint8_t D8;   // GPIO15
	uint8_t V3;   // 3v
} t_pins;

static void blink_led(const t_pins pins) {
	while (1) {
		gpio_set_level(pins.D1, 1);  // Turn the LED on (using D1)
		vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500ms
		gpio_set_level(pins.D1, 0);  // Turn the LED off
		vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500ms
	}
}

static int init_output_pins(const t_pins pins) {
	gpio_config_t io_conf_output;

	io_conf_output.intr_type = GPIO_INTR_DISABLE;
	io_conf_output.mode = GPIO_MODE_OUTPUT;
	io_conf_output.pin_bit_mask = (1ULL << pins.D1);  // Initialize D1 as output
	io_conf_output.pull_down_en = 0;
	io_conf_output.pull_up_en = 0;

	if (gpio_config(&io_conf_output) == ESP_ERR_INVALID_ARG) {
		return ESP_ERR_INVALID_ARG;
	}
	return ESP_OK;
}

void app_main() {
	const t_pins pins = {1, 3, 5, 4, 0, 2, -1, -1, -1, -1, 16, 14, 12, 13, 15, -1};  // Assign your pin numbers

	if (init_output_pins(pins) == ESP_ERR_INVALID_ARG) {
		return;
	}
	blink_led(pins);  // Blink the LED on D1 (GPIO5)
}
