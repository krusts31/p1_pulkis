//#include "protocol_examples_common.h"
#include "lwip/sockets.h"
#include "esp_log.h"
#define PORT CONFIG_EXAMPLE_PORT

static const char	*TAG = "TCP";

extern int global_socket;

void tcp_server(void *pvParameters)
{
	struct sockaddr_in sourceAddr;
	struct sockaddr_in destAddr;
	char rx_buffer[128];
	char addr_str[128];
	int addr_family;
	int ip_protocol;
	int listen_sock;
	int sock;
	int len;
	int opt = 1;
	int err;

	destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(PORT);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;
	inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

	while (1) {
		listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);

		if (listen_sock < 0) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			break;
		}
		ESP_LOGI(TAG, "Socket created");

		setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
		if (err != 0) {
			ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
			break;
		}
		ESP_LOGI(TAG, "Socket binded");

		err = listen(listen_sock, 1);
		if (err != 0) {
			ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
			break;
		}
		ESP_LOGI(TAG, "Socket listening");

		uint addrLen = sizeof(sourceAddr);
		sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
		if (sock < 0) {
			ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
			break;
		}
		ESP_LOGI(TAG, "Socket accepted");

		global_socket = sock;

		while (1) {
			len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0); // Error occured during receiving
			if (len < 0) {
				ESP_LOGE(TAG, "recv failed: errno %d", errno);
				break;
			}
			// Connection closed
			else if (len == 0) {
				ESP_LOGI(TAG, "Connection closed");
				break;
			}
			// Data received
			else {
#ifdef CONFIG_EXAMPLE_IPV6
				// Get the sender's ip address as string
				if (sourceAddr.sin6_family == PF_INET) {
					inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
				} else if (sourceAddr.sin6_family == PF_INET6) {
					inet6_ntoa_r(sourceAddr.sin6_addr, addr_str, sizeof(addr_str) - 1);
				}
#else
				inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
#endif

				rx_buffer[len] = 0; 
				ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
				ESP_LOGI(TAG, "%s", rx_buffer);

				err = send(sock, rx_buffer, len, 0);
				if (err < 0) {
					ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
					break;
				}
			}
		}
	}
	vTaskDelete(NULL);
}
