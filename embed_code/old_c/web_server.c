#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
//#include "protocol_examples_common.h"
#include "nvs.h"
#include "nvs_flash.h"

#include <esp_http_server.h>

static const char *TAG="APP";

/* Function to free context */
void adder_free_func(void *ctx)
{
	ESP_LOGI(TAG, "/adder Free Context function called");
	free(ctx);
}


/* This handler gets the present value of the accumulator */
esp_err_t adder_get_handler(httpd_req_t *req)
{
	/* Log total visitors */
	char outbuf[50];
	unsigned *visitors = (unsigned *)req->user_ctx;
	ESP_LOGI(TAG, "/adder visitor count = %d", ++(*visitors));

	/* Create session's context if not already available */
	if (! req->sess_ctx) {
		ESP_LOGI(TAG, "/adder GET allocating new session");
		req->sess_ctx = malloc(sizeof(int));
		req->free_ctx = adder_free_func;
		*(int *)req->sess_ctx = 0;
	}
	ESP_LOGI(TAG, "/adder GET handler send %d", *(int *)req->sess_ctx);

	/* Respond with the accumulated value */
	snprintf(outbuf, sizeof(outbuf),"%d", *((int *)req->sess_ctx));
	httpd_resp_send(req, outbuf, strlen(outbuf));
	return ESP_OK;
}

/* Maintain a variable which stores the number of times
 * the "/adder" URI has been visited */
static unsigned visitors = 0;

httpd_uri_t adder_get = {
	.uri	  = "/adder",
	.method   = HTTP_GET,
	.handler  = adder_get_handler,
	.user_ctx = &visitors
};

httpd_handle_t start_webserver(void)
{
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
	httpd_handle_t server;

	if (httpd_start(&server, &config) == ESP_OK) {
		ESP_LOGI(TAG, "Registering URI handlers");
		httpd_register_uri_handler(server, &adder_get);
		return server;
	}

	ESP_LOGI(TAG, "Error starting server!");
	return NULL;
}

void stop_webserver(httpd_handle_t server)
{
	httpd_stop(server);
}

static httpd_handle_t server = NULL;

static void disconnect_handler(void* arg, esp_event_base_t event_base, 
							   int32_t event_id, void* event_data)
{
	httpd_handle_t* server = (httpd_handle_t*) arg;
	if (*server) {
		ESP_LOGI(TAG, "Stopping webserver");
		stop_webserver(*server);
		*server = NULL;
	}
}

static void connect_handler(void* arg, esp_event_base_t event_base, 
							int32_t event_id, void* event_data)
{
	httpd_handle_t* server = (httpd_handle_t*) arg;
	if (*server == NULL) {
		ESP_LOGI(TAG, "Starting webserver");
		*server = start_webserver();
	}
}

httpd_handle_t wrapper_start_webserver(void)
{
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
	return start_webserver();
}
