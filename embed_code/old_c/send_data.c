#include "esp_log.h"
#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "data_sender.h"

#include "libft.h"
#include <stdlib.h>

#include <string.h>
#include "mqtt.h"

static const char *TAG = "DATA_SENDER";

//extern int global_socket;

extern uint8_t global_buffer[BUF_SIZE];

static void send_message_to_mqtt(const char *topic, const char *message) {
	if (global_mqtt_client != NULL) {
		esp_mqtt_client_publish(global_mqtt_client, topic, message, 0, 1, 0);
	}
}

/*
static char* extract_value_2(const char* buffer, const char* key) {

	char *start = ft_strnstr((const char*)buffer, key, BUF_SIZE);

	if (start) {
		start = ft_strchr(start, '(');
		if (start) {
			char *end = ft_strrchr(start, 's');
			if (end) {
				end++;
				end++;
				end++;
				size_t len = end - start - 1;
				char *value = ft_calloc(len + 1, 1);  // Add 1 for null-terminator
				if (value == NULL)
					return NULL;
				strncpy(value, start, len);
				return value;
			}
		}
	}
	return NULL;
}
*/

static char* extract_value(const char* buffer, const char* key) {
	// Find the key within the buffer
	char *start = ft_strnstr((const char*)buffer, key, BUF_SIZE);

	if (start) {
		start = ft_strchr(start, '(');
		if (start) {
			start++;  // Move past the '('

			char *end = ft_strchr(start, ')');
			char *unit_marker = ft_strchr(start, '*');

			if (unit_marker && unit_marker < end) {
				end = unit_marker;
			}

			if (end) {
				size_t len = end - start;
				char *value = ft_calloc(len + 1, 1);
				if (value == NULL)
					return NULL;
				strncpy(value, start, len);
				return value;
			}
		}
	}
	return NULL;
}

void data_sender_task(void *pvParameters)
{
	//cJSON *json_obj;
	//char *json_string;
	char *timestamp, *device_id, *status_code;
	char *energy_import_total, *energy_import_tariff1, *energy_import_tariff2, *energy_import_tariff3, *energy_import_tariff4;
	char *energy_export_total, *energy_export_tariff1, *energy_export_tariff2, *energy_export_tariff3, *energy_export_tariff4;
	char *power_import, *power_export;
	char *voltage_phase_1, *voltage_phase_2, *voltage_phase_3;
	char *current_phase_1, *current_phase_2, *current_phase_3;
	char *power_failures, *power_recoveries;
	char *long_power_failures; // *long_power_failures_times
	char *voltage_sag_phase_1, *voltage_sag_phase_2, *voltage_sag_phase_3;
	char *voltage_swell_phase_1, *voltage_swell_phase_2, *voltage_swell_phase_3;

	//int sent_bytes;

	while (1)
	{
		ESP_LOGI(TAG, "Reading and sending data...");

		//if (global_socket != -1)
		//{
			// Create a JSON object
			//json_obj = cJSON_CreateObject();

			// Extract values for OBIS codes
		timestamp = extract_value((char *)global_buffer, "0-0:1.0.0");
		if (timestamp) {
			send_message_to_mqtt("SX_meter/timestamp", timestamp);
			free(timestamp);
		}

		device_id = extract_value((char *)global_buffer, "0-0:96.1.1");
		if (device_id) {
			send_message_to_mqtt("SX_meter/device_id", device_id);
			free(device_id);
		}

		status_code = extract_value((char *)global_buffer, "0-0:96.14.0");
		if (status_code) {
			send_message_to_mqtt("SX_meter/status_code", status_code);
			free(status_code);
		}

		energy_import_total = extract_value((char *)global_buffer, "1-0:1.8.0");
		if (energy_import_total) {
			send_message_to_mqtt("SX_meter/energy_import_total", energy_import_total);
			free(energy_import_total);
		}

		// Similarly, continue sending other values...

		energy_import_tariff1 = extract_value((char *)global_buffer, "1-0:1.8.1");
		if (energy_import_tariff1) {
			send_message_to_mqtt("SX_meter/energy_import_tariff1", energy_import_tariff1);
			free(energy_import_tariff1);
		}

		energy_import_tariff2 = extract_value((char *)global_buffer, "1-0:1.8.2");
		if (energy_import_tariff2) {
			send_message_to_mqtt("SX_meter/energy_import_tariff2", energy_import_tariff2);
			free(energy_import_tariff2);
		}
			 energy_import_tariff3 = extract_value((char *)global_buffer, "1-0:1.8.3");
		if (energy_import_tariff3) {
			send_message_to_mqtt("SX_meter/energy_import_tariff3", energy_import_tariff3);
			free(energy_import_tariff3);
		}

		energy_import_tariff4 = extract_value((char *)global_buffer, "1-0:1.8.4");
		if (energy_import_tariff4) {
			send_message_to_mqtt("SX_meter/energy_import_tariff4", energy_import_tariff4);
			free(energy_import_tariff4);
		}

		energy_export_total = extract_value((char *)global_buffer, "1-0:2.8.0");
		if (energy_export_total) {
			send_message_to_mqtt("SX_meter/energy_export_total", energy_export_total);
			free(energy_export_total);
		}

		energy_export_tariff1 = extract_value((char *)global_buffer, "1-0:2.8.1");
		if (energy_export_tariff1) {
			send_message_to_mqtt("SX_meter/energy_export_tariff1", energy_export_tariff1);
			free(energy_export_tariff1);
		}

		energy_export_tariff2 = extract_value((char *)global_buffer, "1-0:2.8.2");
		if (energy_export_tariff2) {
			send_message_to_mqtt("SX_meter/energy_export_tariff2", energy_export_tariff2);
			free(energy_export_tariff2);
		}

		energy_export_tariff3 = extract_value((char *)global_buffer, "1-0:2.8.3");
		if (energy_export_tariff3) {
			send_message_to_mqtt("SX_meter/energy_export_tariff3", energy_export_tariff3);
			free(energy_export_tariff3);
		}

		energy_export_tariff4 = extract_value((char *)global_buffer, "1-0:2.8.4");
		if (energy_export_tariff4) {
			send_message_to_mqtt("SX_meter/energy_export_tariff4", energy_export_tariff4);
			free(energy_export_tariff4);
		}

		power_import = extract_value((char *)global_buffer, "1-0:1.7.0");
		if (power_import) {
			send_message_to_mqtt("SX_meter/power_import", power_import);
			free(power_import);
		}

		power_export = extract_value((char *)global_buffer, "1-0:2.7.0");
		if (power_export) {
			send_message_to_mqtt("SX_meter/power_export", power_export);
			free(power_export);
		}

		voltage_phase_1 = extract_value((char *)global_buffer, "1-0:32.7.0");
		if (voltage_phase_1) {
			send_message_to_mqtt("SX_meter/voltage_phase_1", voltage_phase_1);
			free(voltage_phase_1);
		}

		voltage_phase_2 = extract_value((char *)global_buffer, "1-0:52.7.0");
		if (voltage_phase_2) {
			send_message_to_mqtt("SX_meter/voltage_phase_2", voltage_phase_2);
			free(voltage_phase_2);
		}

		voltage_phase_3 = extract_value((char *)global_buffer, "1-0:72.7.0");
		if (voltage_phase_3) {
			send_message_to_mqtt("SX_meter/voltage_phase_3", voltage_phase_3);
			free(voltage_phase_3);
		}

		current_phase_1 = extract_value((char *)global_buffer, "1-0:31.7.0");
		if (current_phase_1) {
			send_message_to_mqtt("SX_meter/current_phase_1", current_phase_1);
			free(current_phase_1);
		}

		current_phase_2 = extract_value((char *)global_buffer, "1-0:51.7.0");
		if (current_phase_2) {
			send_message_to_mqtt("SX_meter/current_phase_2", current_phase_2);
			free(current_phase_2);
		}

		current_phase_3 = extract_value((char *)global_buffer, "1-0:71.7.0");
		if (current_phase_3) {
			send_message_to_mqtt("SX_meter/current_phase_3", current_phase_3);
			free(current_phase_3);
		}

		power_failures = extract_value((char *)global_buffer, "0-0:96.7.21");
		if (power_failures) {
			send_message_to_mqtt("SX_meter/power_failures", power_failures);
			free(power_failures);
		}

		power_recoveries = extract_value((char *)global_buffer, "0-0:96.7.9");
		if (power_recoveries) {
			send_message_to_mqtt("SX_meter/power_recoveries", power_recoveries);
			free(power_recoveries);
		}

		long_power_failures = extract_value((char *)global_buffer, "1-0:99.97.0");
		if (long_power_failures) {
			send_message_to_mqtt("SX_meter/long_power_failures", long_power_failures);
			free(long_power_failures);
		}

		voltage_sag_phase_1 = extract_value((char *)global_buffer, "1-0:32.32.0");
		if (voltage_sag_phase_1) {
			send_message_to_mqtt("SX_meter/voltage_sag_phase_1", voltage_sag_phase_1);
			free(voltage_sag_phase_1);
		}

		voltage_sag_phase_2 = extract_value((char *)global_buffer, "1-0:52.32.0");
		if (voltage_sag_phase_2) {
			send_message_to_mqtt("SX_meter/voltage_sag_phase_2", voltage_sag_phase_2);
			free(voltage_sag_phase_2);
		}

		voltage_sag_phase_3 = extract_value((char *)global_buffer, "1-0:72.32.0");
		if (voltage_sag_phase_3) {
			send_message_to_mqtt("SX_meter/voltage_sag_phase_3", voltage_sag_phase_3);
			free(voltage_sag_phase_3);
		}

		voltage_swell_phase_1 = extract_value((char *)global_buffer, "1-0:32.36.0");
		if (voltage_swell_phase_1) {
			send_message_to_mqtt("SX_meter/voltage_swell_phase_1", voltage_swell_phase_1);
			free(voltage_swell_phase_1);
		}

		voltage_swell_phase_2 = extract_value((char *)global_buffer, "1-0:52.36.0");
		if (voltage_swell_phase_2) {
			send_message_to_mqtt("SX_meter/voltage_swell_phase_2", voltage_swell_phase_2);
			free(voltage_swell_phase_2);
		}

		voltage_swell_phase_3 = extract_value((char *)global_buffer, "1-0:72.36.0");
		if (voltage_swell_phase_3) {
			send_message_to_mqtt("SX_meter/voltage_swell_phase_3", voltage_swell_phase_3);
			free(voltage_swell_phase_3);
		}
		vTaskDelay(5000 / portTICK_PERIOD_MS);

		/*
			timestamp = extract_value((char *)global_buffer, "0-0:1.0.0");
			device_id = extract_value((char *)global_buffer, "0-0:96.1.1");
			status_code = extract_value((char *)global_buffer, "0-0:96.14.0");

			energy_import_total = extract_value((char *)global_buffer, "1-0:1.8.0");
			energy_import_tariff1 = extract_value((char *)global_buffer, "1-0:1.8.1");
			energy_import_tariff2 = extract_value((char *)global_buffer, "1-0:1.8.2");
			energy_import_tariff3 = extract_value((char *)global_buffer, "1-0:1.8.3");
			energy_import_tariff4 = extract_value((char *)global_buffer, "1-0:1.8.4");

			energy_export_total = extract_value((char *)global_buffer, "1-0:2.8.0");
			energy_export_tariff1 = extract_value((char *)global_buffer, "1-0:2.8.1");
			energy_export_tariff2 = extract_value((char *)global_buffer, "1-0:2.8.2");
			energy_export_tariff3 = extract_value((char *)global_buffer, "1-0:2.8.3");
			energy_export_tariff4 = extract_value((char *)global_buffer, "1-0:2.8.4");

			power_import = extract_value((char *)global_buffer, "1-0:1.7.0");
			power_export = extract_value((char *)global_buffer, "1-0:2.7.0");

			voltage_phase_1 = extract_value((char *)global_buffer, "1-0:32.7.0");
			voltage_phase_2 = extract_value((char *)global_buffer, "1-0:52.7.0");
			voltage_phase_3 = extract_value((char *)global_buffer, "1-0:72.7.0");

			current_phase_1 = extract_value((char *)global_buffer, "1-0:31.7.0");
			current_phase_2 = extract_value((char *)global_buffer, "1-0:51.7.0");
			current_phase_3 = extract_value((char *)global_buffer, "1-0:71.7.0");

			power_failures = extract_value((char *)global_buffer, "0-0:96.7.21");
			power_recoveries = extract_value((char *)global_buffer, "0-0:96.7.9");
			long_power_failures = extract_value((char *)global_buffer, "1-0:99.97.0");
			//long_power_failures_times = extract_value_2((char *)global_buffer, "0-0:96.7.19");

			voltage_sag_phase_1 = extract_value((char *)global_buffer, "1-0:32.32.0");
			voltage_sag_phase_2 = extract_value((char *)global_buffer, "1-0:52.32.0");
			voltage_sag_phase_3 = extract_value((char *)global_buffer, "1-0:72.32.0");

			voltage_swell_phase_1 = extract_value((char *)global_buffer, "1-0:32.36.0");
			voltage_swell_phase_2 = extract_value((char *)global_buffer, "1-0:52.36.0");
			voltage_swell_phase_3 = extract_value((char *)global_buffer, "1-0:72.36.0");

			send_message_to_mqtt("timestamp", timestamp);
			send_message_to_mqtt("device_id", json_string);
			send_message_to_mqtt("device_id", json_string);

			// Add extracted values to the JSON object
			cJSON_AddStringToObject(json_obj, "timestamp", atoi(timestamp));
			cJSON_AddStringToObject(json_obj, "device_id", atoi(device_id));
			cJSON_AddStringToObject(json_obj, "status_code", atoi(status_code));

			cJSON_AddStringToObject(json_obj, "energy_import_total", atoi(energy_import_total));
			cJSON_AddStringToObject(json_obj, "energy_import_tariff1", atoi(energy_import_tariff1));
			cJSON_AddStringToObject(json_obj, "energy_import_tariff2", atoi(energy_import_tariff2));
			cJSON_AddStringToObject(json_obj, "energy_import_tariff3", atoi(energy_import_tariff3));
			cJSON_AddStringToObject(json_obj, "energy_import_tariff4", atoi(energy_import_tariff4));

			cJSON_AddStringToObject(json_obj, "energy_export_total", atoi(energy_export_total));
			cJSON_AddStringToObject(json_obj, "energy_export_tariff1", atoi(energy_export_tariff1));
			cJSON_AddStringToObject(json_obj, "energy_export_tariff2", atoi(energy_export_tariff2));
			cJSON_AddStringToObject(json_obj, "energy_export_tariff3", atoi(energy_export_tariff3));
			cJSON_AddStringToObject(json_obj, "energy_export_tariff4", atoi(energy_export_tariff4));

			cJSON_AddStringToObject(json_obj, "power_import", atoi(power_import));
			cJSON_AddStringToObject(json_obj, "power_export", atoi(power_export));

			cJSON_AddStringToObject(json_obj, "voltage_phase_1", atoi(voltage_phase_1));
			cJSON_AddStringToObject(json_obj, "voltage_phase_2", atoi(voltage_phase_2));
			cJSON_AddStringToObject(json_obj, "voltage_phase_3", atoi(voltage_phase_3));

			cJSON_AddStringToObject(json_obj, "current_phase_1", atoi(current_phase_1));
			cJSON_AddStringToObject(json_obj, "current_phase_2", atoi(current_phase_2));
			cJSON_AddStringToObject(json_obj, "current_phase_3", atoi(current_phase_3));

			cJSON_AddStringToObject(json_obj, "power_failures", atoi(power_failures));
			cJSON_AddStringToObject(json_obj, "power_recoveries", atoi(power_recoveries));
			cJSON_AddStringToObject(json_obj, "long_power_failures", atoi(long_power_failures));
			//cJSON_AddStringToObject(json_obj, "long_power_failures_times", long_power_failures_times);

			cJSON_AddStringToObject(json_obj, "voltage_sag_phase_1", atoi(voltage_sag_phase_1));
			cJSON_AddStringToObject(json_obj, "voltage_sag_phase_2", atoi(voltage_sag_phase_2));
			cJSON_AddStringToObject(json_obj, "voltage_sag_phase_3", atoi(voltage_sag_phase_3));

			cJSON_AddStringToObject(json_obj, "voltage_swell_phase_1", atoi(voltage_swell_phase_1));
			cJSON_AddStringToObject(json_obj, "voltage_swell_phase_2", atoi(voltage_swell_phase_2));
			cJSON_AddStringToObject(json_obj, "voltage_swell_phase_3", atoi(voltage_swell_phase_3));
			*/

			//json_string = cJSON_Print(json_obj);

			//cJSON_Delete(json_obj);

			//sent_bytes = send(global_socket, json_string, strlen(json_string), 0);

			//cJSON_free(json_string);
			/*
			free(timestamp);
			free(device_id);
			free(status_code);

			free(energy_import_total);
			free(energy_import_tariff1);
			free(energy_import_tariff2);
			free(energy_import_tariff3);
			free(energy_import_tariff4);

			free(energy_export_total);
			free(energy_export_tariff1);
			free(energy_export_tariff2);
			free(energy_export_tariff3);
			free(energy_export_tariff4);

			free(power_import);
			free(power_export);

			free(voltage_phase_1);
			free(voltage_phase_2);
			free(voltage_phase_3);

			free(current_phase_1);
			free(current_phase_2);
			free(current_phase_3);

			free(power_failures);
			free(power_recoveries);
			free(long_power_failures);
			//free(long_power_failures_times);

			free(voltage_sag_phase_1);
			free(voltage_sag_phase_2);
			free(voltage_sag_phase_3);

			free(voltage_swell_phase_1);
			free(voltage_swell_phase_2);
			free(voltage_swell_phase_3);


			if (sent_bytes < 0)
			{
				ESP_LOGE(TAG, "Failed to send data.");
			}
			else
			{
				ESP_LOGI(TAG, "Successfully sent %d bytes", sent_bytes);
			}
			*/
		//}

		// Delay for 5 seconds before the next send
	}
}
