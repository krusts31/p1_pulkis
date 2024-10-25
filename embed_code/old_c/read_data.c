#include "lwip/sockets.h"//not sure why we need this here but we need it. maybe because of global socke
#include "driver/uart.h"
#include "esp_log.h"

#include "libft.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "data_reader.h"

extern uint8_t global_buffer[BUF_SIZE];

static const char *TAG_READER = "DATA_READER";

void init_uart(void) {
	uart_port_t uart_num = UART_NUM_0;
	uint32_t inverse_mask = UART_INVERSE_RXD | UART_INVERSE_TXD;
	esp_err_t result = uart_set_line_inverse(uart_num, inverse_mask);
	if (result != ESP_OK) {
		ESP_LOGE(TAG_READER, "Failed to set UART line inverse");
	}

	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
	
	result = uart_param_config(UART_NUM_0, &uart_config);
	if (result != ESP_OK) {
		ESP_LOGE(TAG_READER, "Failed to configure UART");
	}

	result = uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG_READER, "Failed to install UART driver");
	}
}

void read_p1_hardwareserial(void *pvParameters) {
	int len;

	while (1) {
		ft_memset(global_buffer, 0, BUF_SIZE);
		len = uart_read_bytes(UART_NUM_0, global_buffer, BUF_SIZE, 20 / portTICK_RATE_MS);
		if (len > 0) {
			global_buffer[len] = '\0';
			ESP_LOGI(TAG_READER, "Received string: %s", global_buffer);
		} else {
			ESP_LOGW(TAG_READER, "No data read from UART or read error.");
		}
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}
