#ifndef ESP_DATA_SENDER_H
#define ESP_DATA_SENDER_H

//#define BUF_SIZE 1024

extern int global_socket;

void data_sender_task(void *pvParameters);

#endif
