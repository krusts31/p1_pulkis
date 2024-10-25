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

void app_main()
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(wifi_connect());

    init_uart();

    do_the_thing();

    while (1) {
        ESP_LOGI(TAG, "Looping");
        vTaskDelay(5000 / portTICK_PERIOD_MS);    
    }

    ESP_LOGI(TAG, "Restart");
    fflush(stdout);
    esp_restart();
}

