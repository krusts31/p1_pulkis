#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tcpip_adapter.h"
#include "tcp_server.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs.h"
#include "data_sender.h"
#include "data_reader.h"
#include "driver/uart.h"
#include "wifi.h"
#include "mqtt.h"
#include "mqtt_client.h"
#include "do_the_thing.h"

static const char *TAG = "MAIN";

//int global_socket = -1;

//uint8_t global_buffer[BUF_SIZE];

void app_main()
{
	tcpip_adapter_init();
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/*
	httpd_handle_t *ret_2 = wrapper_start_webserver();
	if (ret_2 != NULL) {
		printf("started web server!");
	} else {
		printf("Failed to start web server.");
		fflush(stdout);
		return ;
	}
	*/

	ESP_ERROR_CHECK(wifi_connect());

	mqtt_start(NULL);
	//init_uart();
	do_the_thing();

	//xTaskCreate(read_p1_hardwareserial, "read_data", 2048, NULL, 6, NULL);
	//xTaskCreate(data_sender_task, "send_data", 2048, NULL, 5, NULL);
	//xTaskCreate(tcp_server, "tcp_server", 2048, NULL, 4, NULL);
	//xTaskCreate(mqtt_start, "mqtt_task", 2048, NULL, 5, NULL);


	//while (1) {
	//	ESP_LOGI(TAG, "Looping");
	//	vTaskDelay(5000 / portTICK_PERIOD_MS);
	//}

	while (1) {
		ESP_LOGI(TAG, "Looping");
		vTaskDelay(5000 / portTICK_PERIOD_MS);	
	}

	ESP_LOGI(TAG, "Restart");
	fflush(stdout);
	esp_restart();
}
