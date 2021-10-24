#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <string.h>
#include <esp_log.h>

#include "max34409.h"
#include "tcp.h"
#include "WiFi_setup.h"


void app_main()
{
    // Init i2cdev library
    ESP_ERROR_CHECK(i2cdev_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    //wifi_init_softap();
    wifi_init_sta();

    //xTaskCreatePinnedToCore(pca9685_test, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);
}