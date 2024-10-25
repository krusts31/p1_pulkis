#include "lwip/sockets.h"
#include "driver/uart.h"
#include "esp_log.h"

#include "libft.h"
#include "mqtt.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "data_reader.h"

#include "mqtt_client.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#define BUF_SIZE 1024

static const char *TAG_READER = "DATA_READER";

typedef struct {
	const char *key;
	const char *mqtt_topic;
} KeyTopicPair;


extern const uint8_t ca_crt_start[] asm("_binary_ca_crt_start");
extern const uint8_t ca_crt_end[] asm("_binary_ca_crt_end");

extern const uint8_t client_crt_start[] asm("_binary_client_crt_start");
extern const uint8_t client_crt_end[] asm("_binary_client_crt_end");

extern const uint8_t client_key_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_end[] asm("_binary_client_key_end");

static KeyTopicPair key_topic_pairs[] = {
    // Timestamp and Device ID
	//{"0-0:1.0.0", "SX_meter/timestamp"},
    //{"0-0:96.1.1", "SX_meter/device_id"},
    {"0-0:96.14.0", "SX_meter/status_code"},

    // Total Energy Import/Export
    {"1-0:1.8.0", "SX_meter/A_PLUS"},    // Total energy import
    {"1-0:2.8.0", "SX_meter/A_MINUS"},   // Total energy export

    // Instantaneous Active Power
    {"1-0:1.7.0", "SX_meter/INST_ACTIV_POWER_A_PLUS"},  // Instantaneous power import
    {"1-0:2.7.0", "SX_meter/INST_ACTIV_POWER_A_MINUS"}, // Instantaneous power export

    // Voltage per Phase
    {"1-0:32.7.0", "SX_meter/U1"},  // Voltage phase 1
    {"1-0:52.7.0", "SX_meter/U2"},  // Voltage phase 2
    {"1-0:72.7.0", "SX_meter/U3"},  // Voltage phase 3

    // Current per Phase
    {"1-0:31.7.0", "SX_meter/I1"},  // Current phase 1
    {"1-0:51.7.0", "SX_meter/I2"},  // Current phase 2
    {"1-0:71.7.0", "SX_meter/I3"},  // Current phase 3

    // Power Failures
    {"0-0:96.7.21", "SX_meter/SHORT_FAILURES"}, // Short power failures
    {"0-0:96.7.9", "SX_meter/LONG_FAILURES"},   // Long power failures

    // Voltage Sags per Phase
    {"1-0:32.32.0", "SX_meter/SAGS_L1"}, // Voltage sag phase 1
    {"1-0:52.32.0", "SX_meter/SAGS_L2"}, // Voltage sag phase 2
    {"1-0:72.32.0", "SX_meter/SAGS_L3"}, // Voltage sag phase 3

    // Voltage Swells per Phase
    {"1-0:32.36.0", "SX_meter/SWELLS_L1"}, // Voltage swell phase 1
    {"1-0:52.36.0", "SX_meter/SWELLS_L2"}, // Voltage swell phase 2
    {"1-0:72.36.0", "SX_meter/SWELLS_L3"}, // Voltage swell phase 3
};

static const int num_key_topic_pairs = sizeof(key_topic_pairs) / sizeof(key_topic_pairs[0]);

static void send_message_to_mqtt(esp_mqtt_client_handle_t *mqtt_client, const char *topic, const char *message) {
	if (mqtt_client != NULL) {
		esp_mqtt_client_publish(*mqtt_client, topic, message, 0, 1, 0);
	}
}

static bool extract_value(const char* buffer, const char* key, char* value_buf, size_t value_buf_size) {
	char *start = ft_strnstr((const char*)buffer, key, BUF_SIZE);
	if (start) {
		start = ft_strchr(start, '(');
		if (start) {
			start++;
			char *end = ft_strchr(start, ')');
			char *unit_marker = ft_strchr(start, '*');
			if (unit_marker && unit_marker < end) {
				end = unit_marker;
			}
			if (end && (size_t)(end - start) < value_buf_size) {
				size_t len = end - start;
				ft_strlcpy(value_buf, start, len);
				value_buf[len] = '\0';
				return true;
			}
		}
	}
	return false;
}

void init_uart(void) {
	uart_port_t uart_num = UART_NUM_0;
	uint32_t inverse_mask = UART_INVERSE_RXD | UART_INVERSE_TXD;
	esp_err_t result = uart_set_line_inverse(uart_num, inverse_mask);
		if (result != ESP_OK) {
		ESP_LOGE(TAG_READER, "Failed to set UART line inverse");
	}
	uart_config_t uart_config = {
		.baud_rate	= 115200,
		.data_bits	= UART_DATA_8_BITS,
		.parity	   = UART_PARITY_DISABLE,
		.stop_bits	= UART_STOP_BITS_1,
		.flow_ctrl	= UART_HW_FLOWCTRL_DISABLE,
	};
	result = uart_param_config(UART_NUM_0, &uart_config);
	if (result != ESP_OK) {
		ESP_LOGE(TAG_READER, "Failed to configure UART");
	}

	// Install UART driver using an event queue here
	result = uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG_READER, "Failed to install UART driver");
	}

	// Optionally, set UART pins if needed (not typically required on ESP8266)
	// UART0 pins are GPIO1 (TX) and GPIO3 (RX) by default.
}

void do_the_thing() {
	uint8_t buffer[BUF_SIZE];
	char value_buf[64]; // Adjust size as needed
	const esp_mqtt_client_config_t mqtt_cfg = {
		.uri = CONFIG_BROKER_URI,
		.cert_pem = (const char *)ca_crt_start,
		.client_cert_pem = (const char *)client_crt_start,
		.transport = MQTT_TRANSPORT_OVER_SSL,
		.skip_cert_common_name_check = true,
		.disable_clean_session = false,
		.client_key_pem = (const char *)client_key_start,
	};

	esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
	//esp_mqtt_client_register_event(global_mqtt_client, ESP_EVENT_ANY_ID, NULL, global_mqtt_client);
	esp_mqtt_client_start(mqtt_client);

	int len;
	while (1) {
		ft_memset(buffer, 0, BUF_SIZE);
		len = uart_read_bytes(UART_NUM_0, buffer, BUF_SIZE - 1, pdMS_TO_TICKS(20));
		ESP_LOGI(TAG_READER, "len: %d", len);
		//send_message_to_mqtt(&mqtt_client, "raw_data", (char *)buffer);
		if (len > 0) {
			ESP_LOGI(TAG_READER, "Received string: %s", buffer);
			for (int i = 0; i < num_key_topic_pairs; i++) {
				ft_memset(value_buf, 0, 64);
				if (extract_value((char *)buffer, key_topic_pairs[i].key, value_buf, sizeof(value_buf))) {
					send_message_to_mqtt(&mqtt_client, key_topic_pairs[i].mqtt_topic, value_buf);
				}
			}
		} else if (len < 0) {
			ESP_LOGE(TAG_READER, "UART read error: %d", len);
		} else {
			ESP_LOGW(TAG_READER, "No data read from UART.");
		}
		vTaskDelay(5000 / portTICK_PERIOD_MS);	
	}
}

