#ifndef ESP_DATA_READER_H
#define ESP_DATA_READER_H

//#define BUF_SIZE 1024

//extern uint8_t global_buffer[BUF_SIZE];

void read_p1_hardwareserial(void *pvParameters);
void init_uart(void);

#endif

