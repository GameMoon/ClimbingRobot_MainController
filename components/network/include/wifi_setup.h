
#ifndef WIFI_H
#define WIFI_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"


#define ESP_WIFI_CHANNEL 1//CONFIG_ESP_WIFI_CHANNEL
#define MAX_STA_CONN 4//CONFIG_ESP_MAX_STA_CONN

#define ESP_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1



static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);

void wifi_init();

void wifi_init_sta(void);
void wifi_init_softap(void);

#endif