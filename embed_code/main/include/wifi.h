#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include "esp_err.h"

esp_err_t wifi_connect(void);

esp_err_t wifi_disconnect(void);

esp_err_t wifi_set_connection_info(const char *ssid, const char *passwd);

#endif
