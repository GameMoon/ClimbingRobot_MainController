#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include "pwm_controller.h"

#define MIN_POSITION 82  //204
#define MAX_POSITION 491 //409
#define MIDDLE_POSITION 285

static const char *ROBOT_TAG = "robot_controller";
static i2c_dev_t pwm_dev;

void robot_controller_setup(){
    pwm_controller_init(&pwm_dev);
}

void robot_controller_set_servos(uint16_t * raw_data,int size){
    uint16_t data[16] = {0};
    data[4] = raw_data[0]; // 1 coxa
    data[5] = raw_data[1]; // 1 femur
    data[6] = raw_data[2]; // 1 tibia

    data[3] = raw_data[3]; // 2 coxa
    data[2] = raw_data[4]; // 2 femur
    data[1] = raw_data[5]; // 2 tibia

    data[12] = raw_data[6]; // 3 coxa
    data[13] = raw_data[7]; // 3 femur
    data[14] = raw_data[8]; // 3 tibia

    data[11] = raw_data[9]; // 4 coxa
    data[10] = raw_data[10]; // 4 femur
    data[9] = raw_data[11]; // 4 tibia

    pwm_controller_set(&pwm_dev, data);
}

static void robot_controller_tcp_msg(const int sock)
{
    int len;
    char rx_buffer[128];
    uint16_t raw_positions[12];

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

            if (len == 24)
            {
                memcpy(raw_positions, rx_buffer, 2 * 12);
                robot_controller_set_servos(raw_positions, 12);
            }
        }
    } while (len > 0);
}

#endif