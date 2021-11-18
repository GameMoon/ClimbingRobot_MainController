#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

// #include "servo_controller.h"
#include "tcp.h"
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


#define SERVO_CONN_PORT 3333
#define CAM_CONN_PORT 3334


static uint8_t robot_status = 1;
uint8_t servo_positions[NUMBER_OF_SERVOS];

TCPParams servoConn;
TCPParams camConn;


void robot_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data);

void robot_controller_init();
void read_callback(uint8_t * data, uint8_t len);
void write_callback(int sock);

void led_status_task(void *pvParameters);

#endif