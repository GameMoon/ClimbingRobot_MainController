#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

// #include "servo_controller.h"
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"


#define MIN_POSITION 82  //204
#define MAX_POSITION 491 //409
#define MIDDLE_POSITION 285

#define NUMBER_OF_SERVOS 12

uint8_t servo_positions[NUMBER_OF_SERVOS];

void robot_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data);

void robot_controller_init();
void read_callback(uint8_t * data, uint8_t len);
void write_callback(int sock);


/*
static void robot_controller_tcp_msg(const int sock)
{
    int len;
    char rx_buffer[128];
    uint8_t positions[13];

    do
    {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE(ROBOT_TAG, "Error occurred during receiving: errno %d", errno);
        }
        else if (len == 0)
        {
            ESP_LOGW(ROBOT_TAG, "Connection closed");
        }
        else
        {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            ESP_LOGI(ROBOT_TAG, "Received %d bytes", len);

            if (len == 13)
            {
                memcpy(positions, rx_buffer, 13);
                //robot_controller_set_servos(raw_positions, 12);
                set_servo_positions(positions);
            }
        }
        read_servo_positions(positions);
        send(sock, positions, 12, 0);

    } while (len > 0);
}
*/
#endif