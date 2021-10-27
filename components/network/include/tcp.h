#ifndef TCP_H
#define TCP_H

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
// #include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#define PORT 3333

// static uint16_t servo_pos[16] = {285};

void (*tcp_read_callback)(uint8_t *data, uint8_t len);
void (*tcp_write_callback)(int sock);


void tcp_server_task(void *pvParameters);

#endif